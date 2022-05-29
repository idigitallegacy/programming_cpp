//
// Created by Михаил on 04.03.2022.
//

#ifndef CPP_LISTENER_EXCEPTION_H
#define CPP_LISTENER_EXCEPTION_H

#endif //CPP_LISTENER_EXCEPTION_H

class ListenerException : std::exception {
private:
    std::string error_;
    int error_id_;

    const char *message_;

public:
    enum LEcode {
        WRONG_ID = 0,
        UNSAFE_INTERVAL
    };

    explicit ListenerException(const std::string &error,
                               const LEcode id)
                                        : error_(error)
                                        , error_id_(id)
                                        , message_() {
        message_ = (error + "\n\tResponse status: " + std::to_string(error_id_)).c_str();
    };

    [[nodiscard]] const char *what() noexcept {
        return message_;
    }

    [[nodiscard]] int get_id() const noexcept {
        return error_id_;
    }

    [[nodiscard]] std::string get_message() const noexcept {
        return error_;
    }
};