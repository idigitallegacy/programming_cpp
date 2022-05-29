//
// Created by Михаил on 21.02.2022.
//

#ifndef LAB1_MEMORY_BUFFER_H
#define LAB1_MEMORY_BUFFER_H

#endif //LAB1_MEMORY_BUFFER_H

class ResponseStorage {
private:
    std::string         buffer_;
    unsigned long long  timestamp_ = 0;

public:
    ResponseStorage() = default;

    void push(const char *needle, const size_t size) {
        for (int i = 0; i < size; ++i) {
            char tmp = needle[i];
            buffer_.push_back(tmp);
        }
        std::chrono::nanoseconds timestamp = std::chrono::system_clock::now().time_since_epoch();
        timestamp_ = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();
    }

    [[nodiscard]] std::string get() const {
        return buffer_;
    }

    [[nodiscard]] unsigned long long last_updated() const {
        return timestamp_;
    }

    void clear() {
        buffer_.clear();
    }
};