#include <jni.h>
#include <string>
#include <fstream>
#include "src/jni/JniHashMap.h"
#include "src/log.h"
#include "PcmReader.h"

// 引用使用C编写的库需要加上 extern "C" 修饰符，否则会报错
#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"

#ifdef __cplusplus
}
#endif


extern "C" JNIEXPORT jstring JNICALL
Java_com_wyl_iaudiopalyer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string ffmpeg_info =
            "版本:\n" + std::string(av_version_info()) + "\n配置:\n" + avcodec_configuration();

    return env->NewStringUTF(ffmpeg_info.c_str());
}


extern "C"
JNIEXPORT jobject JNICALL
Java_com_wyl_iaudiopalyer_AudioMetadata_nativeParse(JNIEnv *env, jobject thiz, jstring file_path) {
    const char *path = env->GetStringUTFChars(file_path, 0);
    AVFormatContext *fmt_ctx = nullptr;
    const AVDictionaryEntry *tag = nullptr;

    av_register_all();

    int ret = avformat_open_input(&fmt_ctx, path, nullptr, nullptr);
    if (ret < 0) {
        LOGE("打开文件 %s 失败：%s", path, av_err2str(ret));
        return nullptr;
    }
//    ret = avformat_find_stream_info(fmt_ctx, nullptr);
//    if (ret < 0) {
//        LOGE("获取流信息失败：%s", av_err2str(ret));
//        return nullptr;
//    }
    std::unique_ptr<JniHashMap> map(new JniHashMap(env));

    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        map->put(tag->key, tag->value);

    // 关闭流
    avformat_close_input(&fmt_ctx);

    env->ReleaseStringUTFChars(file_path, path);

    return map->getHashMap();
}



extern "C"
JNIEXPORT void JNICALL
Java_com_wyl_iaudiopalyer_MainActivity_startReadPcm(JNIEnv *env, jclass clazz, jstring path, jstring pcm_path) {
    auto *pcmReader = new PcmReader(env->GetStringUTFChars(path, 0), env->GetStringUTFChars(pcm_path, 0));
    pcmReader->start();
}