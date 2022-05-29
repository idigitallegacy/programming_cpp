#include <utility>

//
// Created by Михаил on 23.02.2022.
//

#ifndef CPP_JSON_PARSER_H
#define CPP_JSON_PARSER_H

#endif //CPP_JSON_PARSER_H

class Statistics : public RequestHandler
                 , private JSON_parser {
private:
    friend class RequestHandler; // Has to be a friend, because callback_function() is private

    Request request_;
    unsigned long long request_id_;

    struct Meta {
        std::string date;
        unsigned long long last_updated = 0;
        std::string base;

        Meta() = default;

        Meta(std::string _date,
             const unsigned long long _timestamp,
             std::string _base) : date(std::move(_date))
                                , last_updated(_timestamp)
                                , base(std::move(_base)) {}

        Meta(const Meta &cpd) : date(cpd.date)
                              , last_updated(cpd.last_updated)
                              , base(cpd.base) {}

        Meta &operator=(const Meta &cpd) {
            date = cpd.date;
            last_updated = cpd.last_updated;
            base = cpd.base;
            return *this;
        }
    };

    template<typename data_type>
    struct Snapshot {
        std::string key;
        std::vector<data_type> history;
        Meta meta = Meta();

        Snapshot() = default;

        Snapshot(std::string _key,
                 const data_type _value,
                 const Meta &_meta) : key(std::move(_key))
                                    , history(_value)
                                    , meta(_meta) {}

        Snapshot(const Snapshot &cpd) : key(cpd.key)
                                      , history(cpd.history)
                                      , meta(cpd.meta) {}

        Snapshot &operator=(const Snapshot &cpd) {
            key = cpd.key;
            history = cpd.history;
            meta = cpd.meta;
            return *this;
        }
    };

    std::vector<Snapshot<double>> stat_;
    std::vector<std::string> requested_rates;
    bool logging_;

    void callback_function(const Request &request) {
        std::string tmp = request.data();
        const char *tmp_load = tmp.c_str();
        init(tmp_load);
        parse_data();
    }

    void parse_data() {
        std::string array_keys;
        for (auto &i : requested_rates)
            array_keys += i + ',';
        array_keys.erase(array_keys.end() - 1);

        unpack_data(
                {"date", "timestamp", "base", "rates"},
                STRING, INT, STRING, ARRAY, array_keys.c_str(), DOUBLE);

        bool in_line = true;

        for (auto &i : requested_rates) {
            for (auto &j : stat_) {
                if (j.key == i) {
                    j.meta.date = get_item("date").value;
                    j.meta.last_updated = std::stoll(get_item("timestamp").value);
                    j.meta.base = get_item("base").value;

                    j.history.push_back(std::stod(get_item_from_object("rates", i).value));

                    if (logging_)
                        log(j, in_line);
                    in_line = false;
                }
            }
        }
        if (logging_)
            std::cout << '\n';
        JSON_parser::clear();
    }

    double get_average(std::string &rate) {
        double result = 0;
        unsigned long long history_length;
        for (auto &i : stat_) {
            if (i.key == rate) {
                history_length = i.history.size();
                for (auto &j : i.history) {
                    result += j;
                }
            }
        }
        return result / (double) history_length;
    }

    double get_median(std::string &rate) {
        double result = 0;
        for (auto &i : stat_) {
            if (i.key == rate) {
                if (i.history.empty())
                    return 0;
                std::vector<double> tmp = i.history;
                std::sort(tmp.begin(), tmp.end());

                return tmp[tmp.size() / 2];
            }
        }
        return 0;
    }

    void make_table()const {
        std::cout << "\t\t\t\tMeta\t\t\t\t|";
        for (auto &i : requested_rates) {
            std::cout << "\t\t" + i + "\t\t|";
        }
        std::cout << '\n';
        std::cout << "----------------------------------------------------------------|";
        for (auto &i : requested_rates) {
            std::cout << "-------------------------------|";
        }
        std::cout << '\n';
    }

    static void make_log_string(const Snapshot<double> &rate) {
        std::cout << "Date: " << rate.meta.date << ", "
                  << "Last updated: " << rate.meta.last_updated << ", "
                  << "Base:" << rate.meta.base
                  << "\t\t|";
    }

    static void log(const Snapshot<double> &rate, const bool in_line) {
        if (in_line)
            make_log_string(rate);
        std::cout << "\t   "
                  << rate.history[rate.history.size() - 1]
                  << "\t\t|";
    }

public:
    explicit Statistics(const Request &request,
                        const unsigned long long interval = SAFE_INTERVAL,
                        const std::vector<std::string> &needle_currency = std::vector<std::string>{"USD"},
                        const bool logging = false)
                                : RequestHandler()
                                , JSON_parser()
                                , request_(request)
                                , stat_()
                                , logging_(logging) {
        if (request_.last_updated() == 0)
            request_.send();

        requested_rates = needle_currency;
        request_id_ = set_interval(request, interval);

        // Making a beautiful table
        if (logging_) {
            make_table();
        }

        Snapshot<double> tmp_snap;
        for (auto &i : requested_rates) {
            tmp_snap.key = i;
            stat_.push_back(tmp_snap);
        }

        init(request_.data().c_str());
        parse_data();
    }

    void listen() {
        set_listener<Statistics>(this);
    }

    void stop() {
        unset_listener();
    }

    [[nodiscard]] unsigned long long get_request_id() const {
        return request_id_;
    }

    void print() {
        std::cout << "-----------------------------------------------------------------";
        for (auto &i : requested_rates)
            std::cout << "--------------------------------";
        std::cout << '\n';
        std::cout << "\tAverage:\t\t\t\t\t\t|";
        for (auto &i : requested_rates)
            std::cout << "\t   " << get_average(i) << "\t\t|";
        std::cout << '\n';
        std::cout << "\tMedian:\t\t\t\t\t\t\t|";
        for (auto &i : requested_rates)
            std::cout << "\t   " << get_median(i) << "\t\t|";
        std::cout << '\n';
        std::cout << "-----------------------------------------------------------------";
        for (auto &i : requested_rates)
            std::cout << "--------------------------------";
        std::cout << '\n';
    }

    ~Statistics() {
        print();
    }
};