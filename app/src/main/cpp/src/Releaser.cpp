//
// Created by wangyuelin on 2022/10/13.
//

#include <stdexcept>
#include "Releaser.h"
#include "log.h"

Releaser::Releaser(void *buffer, ReleaseFunc release_func) {
    this->m_buffer = buffer;
    this->release_func = release_func;
}

Releaser::~Releaser() {

    if (m_buffer == nullptr) {
        return;
    }
    if (release_func == nullptr) {
        LOGE("release_func is null");
        return;
    }

    // 释放资源
    release_func(m_buffer);
}
