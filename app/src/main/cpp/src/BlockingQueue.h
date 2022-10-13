//
// Created by wangyuelin on 2022/9/18.
//

#ifndef IAUDIOPALYER_BLOCKINGQUEUE_H
#define IAUDIOPALYER_BLOCKINGQUEUE_H

#include <cstdlib>
#include <mutex>
#include <queue>

template<typename T>
class BlockingQueue {
public:
    BlockingQueue();
    ~BlockingQueue();
    // 禁止拷贝构造
    BlockingQueue(BlockingQueue&) = delete;

    /**
     * 添加一个元素，队列满时阻塞
     * @param t
     */
    void put(T &&value);

    /**
     * 获取一个元素，队列为空时阻塞
     * @return
     */
    T take();

    /**
     * 获取队列中元素的个数
     * @return
     */
    size_t size();

    // 清空队列
    void clear();

private:
    int m_maxSize; // 队列的最大容量

private:
    std::mutex m_mutex;
    std::condition_variable m_empty_condition;
    std::condition_variable m_full_condition;
    std::queue <T> m_queue;
};


#endif //IAUDIOPALYER_BLOCKINGQUEUE_H
