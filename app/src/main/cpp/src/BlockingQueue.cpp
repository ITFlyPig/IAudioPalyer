//
// Created by wangyuelin on 2022/9/18.
//

#include "BlockingQueue.h"

template<typename T>
BlockingQueue<T>::BlockingQueue() = default;

template<typename T>
BlockingQueue<T>::~BlockingQueue() = default;

template<typename T>
void BlockingQueue<T>::put(T &&value) {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.size() >= m_maxSize) {
        m_full_condition.wait(lock);
    }
    m_queue.push(std::move(value));
    m_empty_condition.notify_one();
}

template<typename T>
T BlockingQueue<T>::take() {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty()) {
        m_empty_condition.wait(lock);
    }
    T front = std::move(m_queue.pop());
    return front;
}

template<typename T>
size_t BlockingQueue<T>::size() {
    return m_queue.size();
}

template<typename T>
void BlockingQueue<T>::clear() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.clear();
}
