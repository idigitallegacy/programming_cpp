#include <utility>

//
// Created by Михаил on 22.02.2022.
//

#ifndef LAB1_EXCEPTIONS_H
#define LAB1_EXCEPTIONS_H

#endif //LAB1_EXCEPTIONS_H

class RequestException : std::exception {
private:
    std::string error_;
    int error_id_;

    const char *message_;

public:
    explicit RequestException(const std::string &error,
                              const CURLcode id) : error_(error)
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