#include <utility>

//
// Created by Михаил on 01.04.2022.
//

#ifndef CPP_CIRCLE_BUFFER_EXCEPTION_H
#define CPP_CIRCLE_BUFFER_EXCEPTION_H

class CircleBufferException : std::exception {
private:
    int error_id_;
    const char *message_;

public:
    enum CBufferECode {
        FAILED_INIT = 0,
        BUFFER_EMPTY,
        INVALID_OPERATION,
        INVALID_NEW_SIZE
    };

    explicit CircleBufferException(
            const CBufferECode id,
            const std::string &message
            )
            : error_id_(id)
            , message_(message.c_str()) {};

    [[nodiscard]] const char *what() noexcept {
        return message_;
    }

    [[nodiscard]] int get_id() const noexcept {
        return error_id_;
    }
};

#endif //CPP_CIRCLE_BUFFER_EXCEPTION_H