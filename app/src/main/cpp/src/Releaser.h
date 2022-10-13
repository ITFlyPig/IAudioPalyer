//
// Created by wangyuelin on 2022/10/13.
//

#ifndef IAUDIOPALYER_RELEASER_H
#define IAUDIOPALYER_RELEASER_H

// 定义释放的回调函数
typedef void (*ReleaseFunc)(void *);

// 释放器，用于辅助释放内存
class Releaser {
private:
    void *m_buffer; //需要释放的内存
    ReleaseFunc release_func;
public:
    // 函数指针作为参数
    Releaser(void *buffer, ReleaseFunc release_func);

    ~Releaser();
};


#endif //IAUDIOPALYER_RELEASER_H
