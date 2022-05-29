//
// Created by Михаил on 21.04.2022.
//

#ifndef CPP_DOUBLE_QUEUE_H
#define CPP_DOUBLE_QUEUE_H

#include "circle_buffer.h"
#include <vector>
#include "double_queue_exception.h"

template<typename TSource>
class DoubleQueue {
private:
    CircleBuffer<TSource> queue_;
    size_t size_;
    size_t capacity;

public:
    explicit DoubleQueue(const std::vector<TSource> &queue = {})
    : queue_(1)
    , size_(0)
    , capacity(1) {
        if (queue.size() > 1) {
            queue_.resize(queue.size());
            capacity = queue.size();
        }
        for (auto &i : queue)
            queue_.emplace_back(i);
        size_ = queue.size();
    }

    void push_back(const TSource &element) {
        if (size_ + 1 > capacity) {
            capacity *= 2;
            queue_.resize(capacity);
        }
        queue_.emplace_back(element);
        ++size_;
    }

    void push_first(const TSource &element) {
        if (size_ + 1 > capacity) {
            capacity *= 2;
            queue_.resize(capacity);
        }
        queue_.emplace_first(element);
        ++size_;
    }

    TSource top() const {
        return queue_.get_first();
    }

    TSource last() const {
        return queue_.get_last();
    }

    void pop_first() {
        if (size_ == 0)
            throw DQException(DQException::QUEUE_EMPTY, "Queue empty");
        queue_.remove_first();
        --size_;
    }

    void pop_back() {
        if (size_ == 0)
            throw DQException(DQException::QUEUE_EMPTY, "Queue empty");
        queue_.remove_last();
        --size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }
};

#endif //CPP_DOUBLE_QUEUE_H
