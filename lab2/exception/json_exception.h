//
// Created by Михаил on 04.03.2022.
//

#ifndef CPP_JSON_EXCEPTION_H
#define CPP_JSON_EXCEPTION_H

#endif //CPP_JSON_EXCEPTION_H

class JSONException : std::exception {
private:
    std::string error_;
    int error_id_;
    json_error_t error_report_;

    const char *message_;

public:
    enum JSONEcode {
        FAILED_INIT = 0,
        WRONG_OBJECT_POINTER,
        ARRAY_AT_ARRAY,
        KEY_NOT_FOUND,
        OBJECT_NOT_FOUND,
        UNABLE_TO_UNPACK
    };

    explicit JSONException(const std::string &error,
                           const JSONEcode id,
                           const json_error_t json_error_report)
                                    : error_(error)
                                    , error_id_(id)
                                    , error_report_(json_error_report)
                                    , message_() {
        message_ = (
                error +
                "\n\tResponse status: " + std::to_string(error_id_) +
                "\n\tJSON response: " + error_report_.text
        ).c_str();
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