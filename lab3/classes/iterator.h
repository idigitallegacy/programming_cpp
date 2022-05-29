#ifndef LABS_PROGRAMMING_ITERATOR_H
#define LABS_PROGRAMMING_ITERATOR_H

template<typename element_type>
class Iterator {
private:
    element_type *storage_;
    element_type *pointer_;

    size_t capacity_;
public:
    // Used for std::iterator structure (at stl_iterator_base_types.h)
    using difference_type = std::ptrdiff_t;
    using value_type = element_type;
    using pointer = element_type *;
    using reference = element_type &;
    using iterator_category = std::random_access_iterator_tag;

    explicit Iterator(element_type *storage = nullptr, element_type *element = nullptr, const size_t capacity = 0) noexcept
    : storage_(storage)
    , pointer_(element)
    , capacity_(capacity) {
        storage_ = storage;
    }

    Iterator(const Iterator &copied) noexcept
    : storage_(copied.storage_)
    , pointer_(copied.pointer_)
    , capacity_(copied.capacity_) {}

    // Forward iterator requirements
    element_type *operator->() const noexcept {
        return pointer_;
    }

    element_type &operator*() const {
        return *pointer_;
    }

    Iterator &operator++() noexcept {
        pointer_ = pointer_ + 1 < storage_ + capacity_ ? pointer_ + 1 : 0;
        return *this;
    }

    Iterator operator++(int) {
        element_type *tmp = pointer_;
        pointer_ = pointer_ + 1 < storage_ + capacity_ ? pointer_ + 1 : 0;

        return Iterator{storage_, tmp, capacity_};
    }

    // Bidirectional iterator requirements
    Iterator operator--(int) noexcept {
        element_type *tmp = pointer_;
        pointer_ = pointer_ - 1 >= storage_ ? pointer_ - 1 : storage_ + capacity_ - 1;

        return Iterator{storage_, tmp, capacity_};
    }

    Iterator &operator--() noexcept {
        pointer_ = pointer_ - 1 >= storage_ ? pointer_ - 1 : storage_ + capacity_ - 1;
        return *this;
    }

    // Random access iterator requirements

    Iterator &operator=(const size_t value) {
        pointer_ = value;
        return *this;
    }

    bool operator==(const element_type &right) const noexcept {
        return *pointer_ == right;
    }

    bool operator==(const element_type *right) const noexcept {
        return pointer_ == right;
    }

    bool operator==(const Iterator right) const noexcept {
        return pointer_ == right.pointer_;
    }

    bool operator!=(const element_type &right) const noexcept {
        return *pointer_ != right;
    }

    bool operator!=(const element_type *right) const noexcept {
        return pointer_ != right;
    }

    bool operator!=(const Iterator &right) const noexcept {
        return pointer_ != right.pointer_;
    }

    Iterator operator+(const long long number) const noexcept {
        if (number >= 0) {
            size_t index = pointer_ - storage_ + number;
            return Iterator(storage_, storage_ + (index % capacity_), capacity_);
        }
        return *this - (-number);
    }

    Iterator operator-(const size_t number) const noexcept {
        if (number <= 0)
            return *this + (-number);

        if (pointer_ - number >= storage_)
            return Iterator(storage_, pointer_ - number, capacity_);
        else {
            size_t index = number - (pointer_ - storage_);
            size_t modulo = index % capacity_;
            return Iterator(storage_, storage_ + capacity_ - modulo - 1, capacity_);
        }
    }

    difference_type operator-(const Iterator &right) const noexcept {
        return right.pointer_ - pointer_;
    }

    bool operator<(const Iterator &right) const noexcept {
        return pointer_ < right.pointer_;
    }

    bool operator>(const Iterator &right) const noexcept {
        return pointer_ > right.pointer_;
    }

    bool operator<=(const Iterator &right) const noexcept {
        return pointer_ <= right.pointer_;
    }

    bool operator>=(const Iterator &right) const noexcept {
        return pointer_ >= right.pointer_;
    }

    Iterator operator+=(const long long number) noexcept {
        if (number >= 0)
            pointer_ = storage_ + (pointer_ + number) % capacity_;
        else
            return *this -= (-number);
        return *this;
    }

    Iterator operator-=(const long long number) noexcept {
        if (number <= 0)
            return *this += (-number);

        if (pointer_ - number >= storage_)
            pointer_ -= number;
        else {
            pointer_ = (pointer_ + number) % capacity_;
        }
        return *this;
    }

    element_type &operator[](const size_t index) const noexcept {
        size_t new_index = (pointer_ + index) % capacity_;
        return storage_[new_index];
    }
};

template<typename element_type>
class ConstantIterator {
private:
    element_type *storage_;
    element_type *pointer_;

    size_t capacity_;
public:
    // Used for std::iterator structure (at stl_iterator_base_types.h)
    using difference_type = std::ptrdiff_t;
    using value_type = element_type;
    using pointer = element_type *;
    using reference = element_type &;
    using iterator_category = std::random_access_iterator_tag;

    explicit ConstantIterator(element_type *storage = nullptr,
                              element_type *element = nullptr,
                              const size_t capacity = 0) noexcept
    : storage_(storage)
    , pointer_(element)
    , capacity_(capacity) {
        storage_ = storage;
    }

    ConstantIterator(const ConstantIterator &copied) noexcept
    : storage_(copied.storage_)
    , pointer_(copied.pointer_)
    , capacity_(copied.capacity_) {}

    // Forward iterator requirements
    const element_type *operator->() const noexcept {
        return pointer_;
    }

    const element_type &operator*() const {
        return *pointer_;
    }

    const ConstantIterator &operator++() noexcept {
        pointer_ = pointer_ + 1 < storage_ + capacity_ ? pointer_ + 1 : 0;
        return *this;
    }

    ConstantIterator operator++(int) {
        element_type *tmp = pointer_;
        pointer_ = pointer_ + 1 < storage_ + capacity_ ? pointer_ + 1 : 0;

        return Iterator{storage_, tmp, capacity_};
    }

    // Bidirectional iterator requirements
    ConstantIterator operator--(int) noexcept {
        element_type *tmp = pointer_;
        pointer_ = pointer_ - 1 >= storage_ ? pointer_ - 1 : storage_ + capacity_ - 1;

        return Iterator{storage_, tmp, capacity_};
    }

    const ConstantIterator &operator--() noexcept {
        pointer_ = pointer_ - 1 >= storage_ ? pointer_ - 1 : storage_ + capacity_ - 1;
        return *this;
    }

    // Random access iterator requirements

    const ConstantIterator &operator=(const size_t value) {
        pointer_ = value;
        return *this;
    }

    bool operator==(const element_type &right) const noexcept {
        return *pointer_ == right;
    }

    bool operator==(const element_type *right) const noexcept {
        return pointer_ == right;
    }

    bool operator==(const ConstantIterator right) const noexcept {
        return pointer_ == right.pointer_;
    }

    bool operator!=(const element_type &right) const noexcept {
        return *pointer_ != right;
    }

    bool operator!=(const element_type *right) const noexcept {
        return pointer_ != right;
    }

    bool operator!=(const ConstantIterator &right) const noexcept {
        return pointer_ != right.pointer_;
    }

    ConstantIterator operator+(const long long number) const noexcept {
        if (number >= 0) {
            size_t index = pointer_ - storage_ + number;
            return Iterator(storage_, storage_ + (index % capacity_), capacity_);
        }
        return *this - (-number);
    }

    ConstantIterator operator-(const size_t number) const noexcept {
        if (number <= 0)
            return *this + (-number);

        if (pointer_ - number >= storage_)
            return Iterator(storage_, pointer_ - number, capacity_);
        else {
            size_t index = number - (pointer_ - storage_);
            size_t modulo = index % capacity_;
            return Iterator(storage_, storage_ + capacity_ - modulo - 1, capacity_);
        }
    }

    difference_type operator-(const ConstantIterator &right) const noexcept {
        return right.pointer_ - pointer_;
    }

    bool operator<(const ConstantIterator &right) const noexcept {
        return pointer_ < right.pointer_;
    }

    bool operator>(const ConstantIterator &right) const noexcept {
        return pointer_ > right.pointer_;
    }

    bool operator<=(const ConstantIterator &right) const noexcept {
        return pointer_ <= right.pointer_;
    }

    bool operator>=(const ConstantIterator &right) const noexcept {
        return pointer_ >= right.pointer_;
    }

    ConstantIterator operator+=(const long long number) noexcept {
        if (number >= 0)
            pointer_ = storage_ + (pointer_ + number) % capacity_;
        else
            return *this -= (-number);
        return *this;
    }

    ConstantIterator operator-=(const long long number) noexcept {
        if (number <= 0)
            return *this += (-number);

        if (pointer_ - number >= storage_)
            pointer_ -= number;
        else {
            pointer_ = (pointer_ + number) % capacity_;
        }
        return *this;
    }

    element_type operator[](const size_t index) const noexcept {
        size_t new_index = (pointer_ + index) % capacity_;
        return storage_[new_index];
    }
};

#endif //LABS_PROGRAMMING_ITERATOR_H