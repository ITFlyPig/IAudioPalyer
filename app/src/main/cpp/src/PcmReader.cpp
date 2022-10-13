//
// Created by wangyuelin on 2022/9/27.
//

#include "PcmReader.h"
#include "log.h"
#include "libavcodec/avcodec.h"
#include "Releaser.h"

const int error_code = -1;
const int success_code = 0;

PcmReader::PcmReader(std::string mPath, std::string pmc_path) : m_audio_path(std::move(mPath)), m_saved_pmc_path(std::move(pmc_path)) {}

void PcmReader::start() {
    av_register_all();
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&pFormatCtx, m_audio_path.c_str(), nullptr, nullptr);
    if (ret != 0) {
        LOGE("文件：%s打开失败: %s", m_audio_path.c_str(), av_err2str(ret));
        return;
    }
    ret = avformat_find_stream_info(pFormatCtx, nullptr);
    if (ret < 0) {
        LOGE("获取流信息失败：%s", av_err2str(ret));
        return;
    }
    //获取音频流的索引
    int audioStreamIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            break;
        }
    }
    if (audioStreamIndex == -1) {
        LOGE("没有找到音频流");
        return;
    }
    //获取音频流的解码器
    AVCodecContext *pCodecCtx = pFormatCtx->streams[audioStreamIndex]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == nullptr) {
        LOGE("没有找到解码器");
        return;
    }
    //打开解码器
    ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (ret < 0) {
        LOGE("打开解码器失败：%s", av_err2str(ret));
        return;
    }
    //读取音频数据
    AVPacket packet;
    av_init_packet(&packet);
    while (av_read_frame(pFormatCtx, &packet) == 0) {
        if (packet.stream_index == audioStreamIndex) {
            audio_decode_frame(pCodecCtx, packet);
        }
        av_packet_unref(&packet);
    }

    /*
     *计算方式：out_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, 48000) + 1024, 44100, 48000, AV_ROUND_UP);
     * 这里48000是输入音频A的采样率，44100是输出视频B的采样率，这行代码的意思是说，如果转换1024个音频A，能够生成多少个音频B.
     *
     * nb_samples和frame_size区别：
     * nb_samples表示单位时间内，在一定采样率下，samples的总数。
     * 比如： nb_samples = 48000 * 20/ 1000; //48k,20ms 的sample总数
     * frame_size表示固定值opus就是960.
     *
     * 参考文章：https://www.361shipin.com/blog/1503199590172647426
     *
     * 使用avcodec_decode_audio4函数来解码音频，但解码得到的数据类型为float 4bit，而播放器播放的格式一般为S16（signed 16bit）,就需要对解码得到的数据进行转换
     *
     * 声道布局文章：https://its401.com/article/XIAIBIANCHENG/72810495
     * 解码音频实例文章：https://www.likecs.com/show-203604502.html
     *
     * TODO：取出剩余数据
     * 第三个传递NULL，第四个传递0即可将缓存中的全部取出
     * int real_nb_samples = swr_convert(swrCtx,dst_data,dst_nb_samples,NULL,0);
     * if (real_nb_samples <=0) {
            break;
        }
     *
     */

//    max_dst_nb_samples = dst_nb_samples =
//            av_rescale_rnd(src_nb_samples, outSampleRate, inSampleRate, AV_ROUND_UP);

//    av_samples_alloc_array_and_samples(&m_outBuffer, &m_outChannels, 2, 44100, AV_SAMPLE_FMT_S16, 0);


//    av_samples_alloc()
//    av_samples_alloc_array_and_samples()


//    // 释放资源
//    av_free(outBuffer);
//    swr_free(&swrContext);

    if (ret < 0) {
        LOGE("解码音频数据失败：%s", av_err2str(ret));
        return;
    }
    //释放资源
    av_free_packet(&packet);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

}

void release_av(void *ptr) {
    if (ptr) {
        LOGD("释放内存");
        av_freep(ptr);
        ptr = nullptr;
    }
}

void PcmReader::audio_decode_frame(AVCodecContext *p_codec_ctx, AVPacket &packet) const {
    // 初始化转码音频需要的SwrContext
//    SwrContext *swr_context = swr_alloc();
    // 设置输入的采样格式
    enum AVSampleFormat in_sample_fmt = p_codec_ctx->sample_fmt;
    // 设置输出的采样格式
    enum AVSampleFormat dst_sample_fmt = AV_SAMPLE_FMT_S16;
    // 设置输入的采样率
    int in_sample_rate = p_codec_ctx->sample_rate;
    // 设置输出的采样率
    int dst_sample_rate = 44100;
    // 设置输入的声道布局
    uint64_t in_channel_layout = p_codec_ctx->channel_layout;
    // 设置输出的声道布局
    uint64_t dst_channel_layout = AV_CH_LAYOUT_STEREO;
    // 设置转换的参数
    SwrContext *swr_context = swr_alloc_set_opts(nullptr, dst_channel_layout, dst_sample_fmt, dst_sample_rate,
                                                 in_channel_layout, in_sample_fmt, in_sample_rate, 0, nullptr);
    if (swr_context == nullptr) {
        LOGE("初始化转码音频需要的SwrContext失败");
        return;
    }
    // 初始化SwrContext
    int ret = swr_init(swr_context);
    if (ret < 0) {
        LOGE("初始化SwrContext失败：%s", av_err2str(ret));
        return;
    }

    AVFrame *p_frame= av_frame_alloc();// 开始解码AVPacket

    int pkt_size = packet.size;
    while (pkt_size > 0) {

        // 解码
        int got_frame = 0;
        int len = avcodec_decode_audio4(p_codec_ctx, p_frame, &got_frame, &packet);
        // 解码错误
        if (len < 0) {
            LOGE("AVPacket 解码错误：%s， 跳过该帧", av_err2str(len));
            break;
        }
        pkt_size -= len;

        // 重采样
        if (got_frame) {
            int align = 1;
            // 据被转换的音频帧，计算转换后的音频帧的样本数
            int dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_context, in_sample_rate) + p_frame->nb_samples,
                                                dst_sample_rate, in_sample_rate, AV_ROUND_UP);
            int max_dst_nb_samples = dst_nb_samples;

            // 申请输出内存
            int dst_nb_channels = av_get_channel_layout_nb_channels(dst_channel_layout);
            uint8_t **dst_data = nullptr; // 输出缓存
            int dst_linesize = 0;
            // TODO 内存复用，方法内只分配一次内存，参考：https://github.com/nldzsz/ffmpeg-demo/blob/2e1cfcefe49847a069a810e78b19f09118890f19/cppsrc/audio_resample.cpp
            ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels, dst_nb_samples, dst_sample_fmt, align);
            if (ret < 0) {
                LOGE("申请输出内存失败：%s", av_err2str(ret));
                break;
            }

            // 结束本轮循环则释放内存
             Releaser releaser(&dst_data[0], release_av);

            // 重采样
            int nb = swr_convert(swr_context, dst_data, dst_nb_samples,
                                 (const uint8_t **) p_frame->data, p_frame->nb_samples);
            if (nb < 0) {
                LOGE("No data yet, get more frames");
                continue;
            }

            // 重采样后计算实际的缓冲区大小
            int dst_buff_size = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,nb, dst_sample_fmt, align);
            if (dst_buff_size < 0) {
                LOGE("获取重采样后实际的缓冲区大小失败：%s", av_err2str(dst_buff_size));
                break;
            }

            ret = save_pcm_data(*dst_data, dst_buff_size);
            if (ret < 0) {
                LOGE("保存pcm数据失败：%s", strerror(ret));
                break;
            }
        }
    }

    // 释放SwrContext
    swr_free(&swr_context);
    av_free(p_frame);
}

int PcmReader::save_pcm_data(uint8_t *pcmData, int pcmSize) const {
    FILE *fp = fopen(m_saved_pmc_path.c_str(), "ab+");
    if (fp == nullptr) {
        LOGE("打开pcm保存文件：%s失败", m_saved_pmc_path.c_str());
        return errno;
    }
    size_t size = fwrite(pcmData, 1, pcmSize, fp);
    if (size != pcmSize) {
        LOGE("保存pcm数据失败");
        return error_code;
    } else {
        LOGI("写入pcm文件数据size: %d", size);
    }
    fclose(fp);
    return success_code;
}
