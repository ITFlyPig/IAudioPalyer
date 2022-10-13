//
// Created by wangyuelin on 2022/9/28.
//

#ifndef IAUDIOPALYER_PCMFRAME_H
#define IAUDIOPALYER_PCMFRAME_H


class PcmFrame {
private:
    void *m_buffer; //pcm数据
    int m_size; //pcm帧数据的大小

public:
    // 获取一帧数据的大小
    int get_size();

    // 获取一帧数据的指针
    const void *get_buffer();

    PcmFrame(void *mBuffer, int mSize);

    ~PcmFrame() = default;

};


#endif //IAUDIOPALYER_PCMFRAME_H
