#ifndef LABS_PROGRAMMING_CUSTOM_EXCEPTION_H
#define LABS_PROGRAMMING_CUSTOM_EXCEPTION_H

#endif //LABS_PROGRAMMING_CUSTOM_EXCEPTION_H

// General
#define TOO_MUCH_POINTS 11

// Polyline
#define POINT_NOT_FOUND 1
#define POINT_ALREADY_EXISTS 2
#define STRUCTURE_DESTROYING 3
#define TOO_FEW_POINTS 4
#define NOT_A_POLYLINE 12

// Polygon
#define NOT_CONVEX 5

// Correct polygon
#define CHANGING_POINT_IS_NOT_ALLOWED 6
#define INVALID_POINT_MULTIPLYER 7
#define ARBITRARY_POLYGON 8

// Triangle
#define NOT_A_TRIANGLE 9

// Trapezium
#define NOT_A_TRAPEZIUM 10


class FigureException : std::exception {
private:
    std::string error_;
    int error_id_;

public:
    explicit FigureException(const std::string &error, const int id) : error_(error)
                                                                     , error_id_(id) {};

    [[nodiscard]] const char *what() noexcept {
        return error_.c_str();
    }

    [[nodiscard]] int get_id() const noexcept {
        return error_id_;
    }

    [[nodiscard]] std::string get_message() const noexcept {
        return error_;
    }
};



#define VARIABLE_NOT_SUMMABLE 1
#define VARIABLE_NOT_MULTIPLIABLE 2
#define VARIABLE_DIVISION_BY_ZERO 3
#define VARIABLE_IMPOSSIBLE_NAME 4

#define POLYNOM_UNDEFINED_VALUE 5


class AlgebraException : std::exception {
private:
    std::string error_;
    int error_id_;

public:
    explicit AlgebraException(const std::string &error, const int id) : error_(error)
    , error_id_(id) {};

    [[nodiscard]] const char *what() noexcept {
        return error_.c_str();
    }

    [[nodiscard]] int get_id() const noexcept {
        return error_id_;
    }

    [[nodiscard]] std::string get_message() const noexcept {
        return error_;
    }
};