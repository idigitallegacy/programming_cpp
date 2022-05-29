//
// Created by Михаил on 21.04.2022.
//

#ifndef CPP_DOUBLE_QUEUE_EXCEPTION_H
#define CPP_DOUBLE_QUEUE_EXCEPTION_H

class DQException : std::exception {
private:
    int error_id_;
    const char *message_;

public:
    enum DQECode {
        FAILED_INIT = 0,
        QUEUE_EMPTY
    };

    explicit DQException(
            const DQECode id,
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

#endif //CPP_DOUBLE_QUEUE_EXCEPTION_H
