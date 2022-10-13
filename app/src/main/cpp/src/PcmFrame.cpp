//
// Created by wangyuelin on 2022/9/28.
//

#include "PcmFrame.h"

PcmFrame::PcmFrame(void *mBuffer, int mSize) : m_buffer(mBuffer), m_size(mSize) {}

int PcmFrame::get_size() {
    return m_size;
}

const void *PcmFrame::get_buffer() {
    return m_buffer;
}


