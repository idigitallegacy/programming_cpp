#include <utility>

//
// Created by Михаил on 02.03.2022.
//

#ifndef CPP_JSON_PARSER_H
#define CPP_JSON_PARSER_H

#endif //CPP_JSON_PARSER_H

class JSON_parser {
protected:
    enum data_type {
        STRING,
        INT,
        DOUBLE,
        ARRAY
    };

    /**
     * A structure to represent simple types: int, double, string, etc. \n
     * As at the real JS, data (JSON_item.value) is represented as a string.
     */
    struct JSON_item {
        std::string key;
        std::string value;

        explicit JSON_item(std::string _key, std::string _value = "")
                                : key(std::move(_key))
                                , value(std::move(_value)) {}

        JSON_item() : key(), value() {}
    };

    /**
    * A structure to represent array and object types. \n
    * JSON_object_item.value will return an std::vector<JSON_item>
    */
    struct JSON_object_item {
        std::string key;
        std::vector<JSON_item> value;

        explicit JSON_object_item(std::string _key,
                                  std::vector<JSON_item> _value = std::vector<JSON_item>{})
                                            : key(std::move(_key))
                                            , value(std::move(_value)) {}

        JSON_object_item()
                : key("undefined"), value() {}
    };

    /// A pointer to json (provided by init() method).
    json_t *json_root_;
    /// An error container.
    json_error_t error_;

    /**
     * A function to initialize json_root_ field.
     * @param root A JSON string (e.g. " \b { "foo": 1, "b": "0" \b } ")
     */
    void init(const char *root) {
        json_root_ = json_loads(root, JSON_EMBED, &error_);
        if (!json_root_)
            throw JSONException("Unable to reach " + std::string(root) + " at JSON_parser.h:44",
                                JSONException::FAILED_INIT,
                                error_);
    }

    JSON_parser() : data_()
                  , objects_data_()
                  , json_root_()
                  , error_() {}

    /**
     * A function to unpack objects. Void * return type is used due to
     * (JSON_object_item *) is unavailable. If keyN is
     * an array, JSON_item.value == std::string(JSON_object_item *).
     * @param array (json_t) an array object to unpack.
     * @param parent_key (std::string)
     * @param keys (const char *) array keys that you need to unpack. Must be split by a ','
     * symbol \b without \b whitespaces
     * @param type (data_type -> int) data type enumeration (e. g. 0 == STRING).
     * @return a pointer to memory where JSON_object_item is stored.
     * @example JSON_object_item *foo = (JSON_object_item *) unpack_array(foo, "foo", "bar", JSON_parser::INT);
     */
    void *unpack_array(json_t *array,
                       const std::string &parent_key,
                       const char *keys,
                       const int type) {
        if (!array)
            throw JSONException(
                    "Unable to reach object at JSON_parser.h:51",
                    JSONException::WRONG_OBJECT_POINTER,
                    error_
            );
        if (!json_is_object(array))
            throw JSONException(
                    "Given pointer is not an object at JSON_parser.h:51",
                    JSONException::WRONG_OBJECT_POINTER,
                    error_
            );

        JSON_object_item array_container(parent_key);
        json_t *bubble = json_root_; // bubble is needed to execute item_constructor->data_proccessor()
        std::string keys_init; // a copy of keys to split keys easily
        std::vector<std::string> keys_array; // split keys storage
        size_t pos; // a temp pointer needed to split keys
        JSON_object_item *reference; // a pointer to unpacked data (will be returned)

        // Temporarily change the object to perform: root = root->object
        // (e.g. root = {foo : {"bar": true}}. If key == "bar" makes root = {"bar": true}
        json_root_ = array;

        // Split keys by ',' symbol. Given array must be represented as "key1,key2,...,keyN"
        // WITHOUT whitespaces.
        keys_init = keys;
        while ((pos = keys_init.find(',')) != std::string::npos) {
            keys_array.push_back(keys_init.substr(0, pos));
            keys_init.erase(0, pos + 1);
        }
        keys_array.push_back(keys_init);

        // Unpack array into array_container.value
        for (auto &i : keys_array) {
            if (type != ARRAY) {
                JSON_item item = item_constructor(i, type);
                array_container.value.push_back(item);
            } else {
                throw JSONException(
                        "Unable to unpack (2+)-leveled arrays",
                        JSONException::ARRAY_AT_ARRAY,
                        error_
                );
            }
        }

        // Write an object to data storage
        objects_data_.push_back(array_container);
        reference = &objects_data_[objects_data_.size() - 1];

        // Go back to parent root. As at the example above, root = {"bar": true}
        // makes root = {foo : {"bar": true}}
        json_root_ = bubble;

        return reference;
    }

    /**
     * A function to unpack provided JSON string.
     * @param keys Keys to be unpacked
     * @param ... JSON_parser::data_type \n
     *            Type of each key. Should be passed as {data_type(key1), data_type(key2), ...}
     * @param ... JSON_parser::ARRAY, char *keys, JSON_parser::data_type \n
     *            If keyN is an array, there are some extra parameters:
     *            char *keys - root[keyN] keys to be unpacked and
     *            JSON_parser::data_type - type of elements stored at the array.
     *            \b One \b parameter for the whole array.
     * @example
     * unpack_data(std::vector<std::string>{ "base", "timestamp", "rates" }, STRING, INT,
     *                                                                      ARRAY, "USD,EUR", DOUBLE);
     */
    void unpack_data(const std::vector<std::string> keys, ...) { // Passed by value due to va_start(argptr, keys)
        va_list argptr;
        int type;

        json_t *tmp_root;
        const char *array_keys;
        int data_t;

        std::stringstream sstream;

        va_start(argptr, keys);
        for (auto &i : keys) {
            type = va_arg(argptr, int);
            if (type != ARRAY) {
                data_.push_back(item_constructor(i, type));
            } else {
                tmp_root = data_proccessor<json_t *>("{s:o}", i);

                array_keys = va_arg(argptr, const char *);
                data_t = va_arg(argptr, int);

                auto *address = (JSON_object_item *) unpack_array(tmp_root,
                                                                  i,
                                                                  array_keys,
                                                                  data_t);
                sstream << address;
                data_.emplace_back(JSON_item{i, sstream.str()});
            }
        }
        va_end(argptr);
    }

    /**
     * A function to get item by key.
     * @param key
     * @return JSON_parser::JSON_item \n
     *         A JSON item.
     */
    [[nodiscard]] JSON_item get_item(const std::string &key) const {
        for (auto &i : data_)
            if (i.key == key)
                return i;
        throw JSONException("JSON key " + key + " not found at JSON_parser.h:120",
                            JSONException::KEY_NOT_FOUND,
                            error_);
    }

    /**
     * A function to get object by key.
     * @param key
     * @return JSON_parser::JSON_object_item \n
     *         A JSON object item.
     */
    [[nodiscard]] JSON_object_item get_object(const std::string &key) const {
        for (auto &i : data_)
            if (i.key == key) {
                std::string ref(i.value);
                std::stringstream ref_ss(ref);

                void *tmp_pointer;
                ref_ss >> tmp_pointer;

                auto *pointer = (JSON_object_item *) tmp_pointer;
                return *pointer;
            }
        throw JSONException("JSON object " + key + " not found at JSON_parser.h:127",
                            JSONException::OBJECT_NOT_FOUND,
                            error_);
    }

    /**
     * A function to get item by key stored at object.
     * @param object_key an object key that stores item_key
     * @param item_key an item key that stored at object_key
     * @return JSON_parser::JSON_item \n
     *         A JSON item.
    */
    [[nodiscard]] JSON_item get_item_from_object(const std::string &object_key,
                                                 const std::string &item_key) const {
        JSON_object_item object = get_object(object_key);
        for (auto &i : object.value)
            if (i.key == item_key)
                return i;
        throw JSONException("JSON key " + item_key + " at " + object_key + " not found at JSON_parser.h:142",
                            JSONException::KEY_NOT_FOUND,
                            error_);
    }

    void clear() {
        data_.clear();
        objects_data_.clear();
        delete[] json_root_;
    }

private:
    /// JSON items storage
    std::vector<JSON_item> data_;
    /// JSON objects storage
    std::vector<JSON_object_item> objects_data_;

    /**
     * A function to unpack data.
     * @tparam data_t data type form enum data_type
     * @param format format as Jansson requires (e.g. {s:i}
     * @param key
     * @return data_t unpacked value.
     */
    template<typename data_t>
    data_t data_proccessor(const std::string &format, const std::string &key) {
        data_t data_storage;
        int result;

        result = json_unpack_ex(json_root_, &error_, NULL,
                                format.c_str(),
                                key.c_str(), &data_storage);

        if (result != 0)
            throw JSONException("Unable to unpack " + key + " as " + format + " at JSON_parser.h:162",
                                JSONException::UNABLE_TO_UNPACK,
                                error_);
        return data_storage;
    }

    /**
     * A function to perform data_processor(). Switches type of an element and
     * returns an unpacked JSON item.
     * @param key
     * @param type
     * @return JSON_item JSON item
     */
    JSON_item item_constructor(const std::string &key, const int type) {
        JSON_item item(key);
        switch ((data_type) type) {
            case STRING: {
                item.value = data_proccessor<const char *>("{s:s}", key);
                break;
            }
            case INT: {
                item.value = std::to_string(data_proccessor<int>("{s:i}", key));
                break;
            }
            case DOUBLE: {
                item.value = std::to_string(data_proccessor<double>("{s:f}", key));
                break;
            }
            default: {
                break;
            }
        }
        return item;
    }
};