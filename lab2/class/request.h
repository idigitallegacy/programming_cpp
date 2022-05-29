#include <utility>

//
// Created by Михаил on 21.02.2022.
//

#ifndef LAB1_REQUEST_H
#define LAB1_REQUEST_H

#endif //LAB1_REQUEST_H

#define SAFE_INTERVAL 500 // in millis

class Request {
private:
    struct Options {
        // Default options
        unsigned long long connection_timeout = 5000;
        std::string URL = "https://itmo.ru/ru/";

        // Error options
        char *error_buffer = nullptr;
        long keep_alive_on_error = 0;

        // Network options
        long path_as_is = 0;
        long port = 0;
        long protocols = CURLPROTO_HTTP | CURLPROTO_HTTPS;
    };

    CURL *handler_;
    CURLcode curl_status_;
    Options options_;
    ResponseStorage response_;

    static size_t
    write_response(const void *contents,
                   const size_t size,
                   const size_t nmemb,
                   const void *userp) noexcept {
        size_t realsize = size * nmemb;
        auto *buffer = (ResponseStorage *) userp;
        auto *body = (char *) contents;

        buffer->push(body, nmemb);
        return realsize;
    }

    void setup() {
        curl_easy_setopt(handler_, CURLOPT_URL, options_.URL.c_str());
        curl_easy_setopt(handler_, CURLOPT_WRITEFUNCTION, &write_response);
        curl_easy_setopt(handler_, CURLOPT_WRITEDATA, (void *) &response_);
        curl_easy_setopt(handler_, CURLOPT_CONNECTTIMEOUT_MS, options_.connection_timeout);
        curl_easy_setopt(handler_, CURLOPT_ERRORBUFFER, options_.error_buffer);
        curl_easy_setopt(handler_, CURLOPT_KEEP_SENDING_ON_ERROR, options_.keep_alive_on_error);
        curl_easy_setopt(handler_, CURLOPT_PATH_AS_IS, options_.path_as_is);
        curl_easy_setopt(handler_, CURLOPT_PORT, options_.port);
        curl_easy_setopt(handler_, CURLOPT_PROTOCOLS, options_.protocols);
        if (curl_status_ != CURLE_OK)
            throw RequestException("Failed curl_easy_setopt() at request.h:(47-55)", curl_status_);
    }

public:
    typedef Options RequestOptions;

    Request()
            : curl_status_(), options_(), response_() {
        handler_ = curl_easy_init();
        if (!handler_)
            throw RequestException("Failed handler_ initialization at request.h:65", curl_status_);
        setup();
    }

    void configure(const RequestOptions &options) noexcept {
        options_ = options;
    }

    void send() {
        response_.clear();
        setup();
        curl_status_ = curl_easy_perform(handler_);
        if (curl_status_ != CURLE_OK)
            throw RequestException("Failed to perform handler_ at request.h:77",
                                   curl_status_);
        curl_easy_cleanup(handler_);
    }

    [[nodiscard]] CURLcode status() const noexcept {
        return curl_status_;
    }

    [[nodiscard]] std::string data() const noexcept {
        return response_.get();
    }

    [[nodiscard]] unsigned long long last_updated() const noexcept {
        return response_.last_updated();
    }

    Request(const Request &cpd) noexcept {
        curl_status_ = cpd.curl_status_;
        options_ = cpd.options_;
        response_ = cpd.response_;
        handler_ = curl_easy_init();
        setup();
    }
};
