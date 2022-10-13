package com.wyl.iaudiopalyer;

import java.util.HashMap;

/**
 * Time  :2022/9/17
 * Author:yuelinwang
 * Desc  : 音频的元数据
 */
public class AudioMetadata {
    public String filePath;
    private HashMap<String, String> metadata;

    public AudioMetadata(String filePath) {
        this.filePath = filePath;
    }

    public boolean parse() {
        metadata = nativeParse(filePath);
        return metadata != null;
    }

    /**
     * 本地解析得到音频的元数据
     *
     * @param filePath
     * @return
     */
    private native HashMap<String, String> nativeParse(String filePath);

    public String getChannelCount() {
        if (metadata == null) {
            return "";
        }
        return metadata.get("channelCount");
    }
}


