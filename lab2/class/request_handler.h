#include <utility>

//
// Created by Михаил on 21.02.2022.
//

#ifndef LAB1_PARSER_H
#define LAB1_PARSER_H

#endif //LAB1_PARSER_H

class RequestHandler {
private:
    std::mutex thread_mutex_;
    bool is_listening_;

    struct RequestItem {
        unsigned long long id;
        unsigned long long interval = SAFE_INTERVAL;
        unsigned long long timestamp = 0;
        Request request;

        RequestItem(const unsigned long long _id,
                    const unsigned long long _interval,
                    const Request &_request) : id(_id)
                                             , interval(_interval)
                                             , request(_request)
                                             , timestamp(_request.last_updated()) {}
    };

    template<class Handler_class_name>
    static void interval_listener(bool &listener_condition,
                                  std::vector<RequestItem> &list,
                                  Handler_class_name *object,
                                  std::mutex &listener_mutex) {
        while (true) {
            listener_mutex.lock();
            if (!listener_condition) {
                listener_mutex.unlock();
                break;
            }
            std::vector<RequestItem> copied_list = list;
            listener_mutex.unlock();

            std::chrono::nanoseconds temp_timestamp = std::chrono::system_clock::now().time_since_epoch();
            unsigned long long timestamp =
                    std::chrono::duration_cast<std::chrono::milliseconds>(temp_timestamp).count();

            for (auto &i : copied_list) {
                if (i.interval == 0)
                    continue;
                unsigned long long difference = timestamp - i.timestamp;
                if (difference >= i.interval) {
                    i.request.send();
                    i.timestamp = timestamp;
                    object->callback_function(i.request);
                }
            }

            listener_mutex.lock();
            list = copied_list;
            listener_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(SAFE_INTERVAL));
        }
    }

protected:
    std::vector<RequestItem> list_of_requests_;

    template<class Handler_class_name>
    void set_listener(Handler_class_name *object) {
        if (!is_listening_) {
            thread_mutex_.lock();
            is_listening_ = true;
            thread_mutex_.unlock();

            std::thread listener_thread(interval_listener<Handler_class_name>,
                                        std::ref(is_listening_),
                                        std::ref(list_of_requests_),
                                        std::move(object),
                                        std::ref(thread_mutex_));
            listener_thread.detach();
        }
    }

    void unset_listener() noexcept {
        if (is_listening_) {
            thread_mutex_.lock();
            is_listening_ = false;
            thread_mutex_.unlock();
        }
    }

public:
    RequestHandler() noexcept {
        list_of_requests_ = {};
        is_listening_ = false;
    }

    unsigned long long
    set_interval(const Request &request, const unsigned long long interval = SAFE_INTERVAL) noexcept {
        unsigned long long id = list_of_requests_.size();
        list_of_requests_.emplace_back(id, interval, request);
        return id;
    }

    virtual void update_interval(const unsigned long long id, const unsigned long long interval) {
        if (id > list_of_requests_.size())
            throw ListenerException("Wrong request id at request_handler.h:103.",
                                    ListenerException::WRONG_ID);
        if (interval > SAFE_INTERVAL)
            throw ListenerException("New interval is not safe to use at request_handler.h:103.",
                                    ListenerException::UNSAFE_INTERVAL);

        list_of_requests_[id].interval = interval;
    }

    virtual void clear_interval(const unsigned long long id) {
        if (id > list_of_requests_.size())
            throw ListenerException("Wrong request id at request_handler.h:112.",
                                    ListenerException::WRONG_ID);
        list_of_requests_[id].interval = 0;
    }

    Request get_request(const unsigned long long id) {
        if (id > list_of_requests_.size())
            throw ListenerException("Wrong request id at request_handler.h:1118.",
                                    ListenerException::WRONG_ID);
        return list_of_requests_[id].request;
    }
};