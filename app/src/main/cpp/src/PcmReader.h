//
// Created by wangyuelin on 2022/9/27.
//

#ifndef IAUDIOPALYER_PCMREADER_H
#define IAUDIOPALYER_PCMREADER_H


#include <string>

#include <utility>


#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus
}
#endif

// pcm 数据读取器
class PcmReader {
private:
    std::string m_audio_path; //音频文件路径
    std::string m_saved_pmc_path; // 保存pcm文件的路径

public:

    PcmReader(std::string mPath, std::string pmc_path);

    ~PcmReader() = default;

    // 开始解析并读取pcm数据
    void start();

private:
    /**
     * 解码音频
     * @param p_codec_ctx
     * @param packet
     */
    void audio_decode_frame(AVCodecContext *p_codec_ctx, AVPacket &packet) const;

    /**
     * 保存pcm数据
     * @param pcmData
     * @param pcmSize
     */
    int save_pcm_data(uint8_t *pcmData, int pcmSize) const;

    /**
     * 打开文件
     */
    void open_file() const;

};


#endif //IAUDIOPALYER_PCMREADER_H
