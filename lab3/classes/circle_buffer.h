#include <iostream>
#include "circle_buffer_exception.h"

#ifndef CPP_CIRCLE_BUFFER
#define CPP_CIRCLE_BUFFER

template<typename TSource>
class CircleBuffer {
private:
    /// storage_ is an array of element_type elements
    TSource *storage_;

    /// Pointer to the first added element (0 if buffer is empty). Shouldn't be unsigned due to --first_ possibly <= 0
    long long first_;
    /// Pointer to the last added element (0 if buffer is empty). Shouldn't be unsigned due to --last_ possibly <= 0
    long long last_;

    /// A counter for stored elements
    size_t size_;
    /// Max possible amount of stored elements
    size_t capacity_;

    /// Safely resizes the circle buffer, saving it's order and cleaning up unused memory.
    void safe_resize(const long long new_size) {
        // Dump all the private fields as local variables
        if (new_size < 0)
            throw CircleBufferException(CircleBufferException::FAILED_INIT, "Negative new size");

        const TSource *old_storage = storage_;
        const size_t old_first = first_;
        const size_t old_last = last_;

        const size_t old_capacity = capacity_;

        // Allocating memory for new buffer
        auto *new_storage = new TSource[new_size];
        // index must be -1 due to storage[++index] usage
        long long index = -1;

        // This condition improves performance in case of empty buffer with big capacity (O(1) instead of O(N))
        if (size_ == 0) {
            capacity_ = new_size;
            storage_ = new_storage;
            delete[] old_storage; // cleaning up unused memory
            return;
        }

        if (old_first < old_last) {
            // copyng in forward direction
            for (unsigned long long i = old_first; i < old_last; ++i)
                new_storage[++index] = old_storage[i];
            for (unsigned long long i = old_last; i < old_capacity; ++i)
                new_storage[++index] = old_storage[i];
            for (unsigned long long i = 0; i < old_first; ++i)
                new_storage[++index] = old_storage[i];
        } else {
            // copying in reversed direction
            for (unsigned long long i = old_first; i < old_capacity; ++i)
                new_storage[++index] = old_storage[i];
            for (unsigned long long i = 0; i < old_last; ++i)
                new_storage[++index] = old_storage[i];
            for (unsigned long long i = old_last; i < old_first; ++i)
                new_storage[++index] = old_storage[i];
        }

        first_ = 0;
        last_ = index;
        storage_ = new_storage;

        capacity_ = new_size;
        delete[] old_storage; // cleaning up unused memory
    }

public:
    #include "iterator.h"

    explicit CircleBuffer(const size_t capacity = 1)
    : first_(0)
    , last_(0)
    , size_(0)
    , capacity_(capacity)
    , storage_(new TSource[capacity]) {}

    CircleBuffer(const CircleBuffer &source)
    : storage_(source.storage_)
    , size_(source.size_)
    , capacity_(source.capacity_)
    , first_(source.first_)
    , last_(source.last_) {}

    /// Checks if buffer is empty (if it stores at least one emelent)
    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    /// @returns number of elements
    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }

    /**
     * @returns buffer capacity.
     */
    [[nodiscard]] size_t capacity() const noexcept {
        return capacity_;
    }

    /// Places the element after the last one. If circle buffer is full, replaces the first added elements.
    void emplace_back(const TSource &element) {
        if (size_ == 0) {
            storage_[last_] = element;
            ++size_;
            return;
        }

        size_ = size_ + 1 > capacity_ ? capacity_ : size_ + 1;

        ++last_;
        if (last_ >= capacity_)
            last_ = 0;
        if (size_ > 1) {
            if (last_ == first_)
                ++first_;
            if (first_ >= capacity_)
                first_ = 0;
        }
        storage_[last_] = element;

    }

    /// Places the element before the first one. If circle buffer is full, replaces the first added elements.
    void emplace_first(const TSource &element) noexcept {
        if (size_ == 0) {
            storage_[first_] = element;
            ++size_;
            return;
        }

        --first_;
        size_ = size_ + 1 > capacity_ ? capacity_ : size_ + 1;

        if (first_ < 0)
            first_ = capacity_ - 1;
        if (size_ > 1) {
            if (first_ == last_)
                --last_;
            if (last_ < 0)
                last_ = capacity_ - 1;
        }

        storage_[first_] = element;
    }

    /// Removes last added element
    void remove_last() {
        if (empty())
            throw CircleBufferException(CircleBufferException::BUFFER_EMPTY, "Buffer is empty.");
        storage_[last_] = ~TSource();

        --last_;
        --size_;

        if (last_ < 0)
            last_ = capacity_ - 1;
        if (size_ > 1) {
            if (last_ == first_)
                --first_;
            if (first_ < 0)
                first_ = capacity_ - 1;
        }

        if (empty()) {
            first_ = 0;
            last_ = 0;
        }
    }

    /// Removes first added element
    void remove_first() {
        if (empty())
            throw CircleBufferException(CircleBufferException::BUFFER_EMPTY, "Buffer is empty.");
        storage_[first_] = ~TSource();

        ++first_;
        --size_;

        if (first_ >= capacity_)
            first_ = 0;
        if (size_ > 1) {
            if (first_ == last_)
                ++last_;
            if (last_ >= capacity_)
                last_ = 0;
        }

        if (empty()) {
            first_ = 0;
            last_ = 0;
        }
    }

    /// @returns first added element
    TSource get_first() const {
        if (empty())
            throw CircleBufferException(CircleBufferException::BUFFER_EMPTY, "Buffer is empty.");
        return storage_[first_];
    }

    /// @returns last added element
    TSource get_last() const {
        if (empty())
            throw CircleBufferException(CircleBufferException::BUFFER_EMPTY, "Buffer is empty.");
        return storage_[last_];
    }

    /**
     * @returns element stored at first_ + index position.
     */
    TSource operator[](const size_t index) const noexcept {
        if (size_ == capacity_)
            return storage_[(first_ + index) % capacity_];
        else
            return storage_[(first_ + index) % last_];
    }

    /// Changes the Circle Buffer capacity
    void resize(const size_t new_size) {
        if (new_size < capacity_)
            throw CircleBufferException(CircleBufferException::INVALID_NEW_SIZE, "New buffer size must be greater than or equal to an old size");
        else if (new_size == capacity_)
            return;
        safe_resize(new_size);
    }

    /// @returns Random Access iterator to the first added element
    Iterator<TSource> begin() noexcept {
        return Iterator<TSource>(storage_, storage_ + first_, capacity_);
    }

    ConstantIterator<TSource> begin() const noexcept {
        return ConstantIterator<TSource>(storage_, storage_ + first_, capacity_);
    }

    /// @returns Random Access iterator to the element added after the last one
    Iterator<TSource> end() noexcept {
        return Iterator<TSource>(storage_, storage_ + ((last_ + 1) % capacity_), capacity_);
    }

    ConstantIterator<TSource> end() const noexcept {
        return ConstantIterator<TSource>(storage_, storage_ + ((last_ + 1) % capacity_), capacity_);
    }

    void print() {
        for (long long i = 1e6-10; i < capacity_; ++i) {
            std::cout << storage_[i] << ' ';
        }
    }

    ~CircleBuffer() {
        if (capacity_ > 0)
            delete[] storage_;
    }
};

#endif