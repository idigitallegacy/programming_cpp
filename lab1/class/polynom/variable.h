#include <utility>

//
// Created by Михаил on 18.02.2022.
//

#ifndef LAB1_VARIABLE_H
#define LAB1_VARIABLE_H

#endif //LAB1_VARIABLE_H

class Variable {
private:
    double coefficient_;
    std::string variable_name_;
    double power_;

    static bool check_if_summable(const Variable &left, const Variable &right) {
        return left.variable_name_ == right.variable_name_ && left.power_ == right.power_;
    }

    static bool check_if_multipliable(const Variable &left, const Variable &right) {
        return left.variable_name_ == right.variable_name_;
    }

    static bool check_if_divisible(const Variable &left, const Variable &right) {
        return left.variable_name_ == right.variable_name_ && right.coefficient_ != 0;
    }
public:
    [[nodiscard]] std::string get_name() const {
        return variable_name_;
    }

    [[nodiscard]] double get_power() const {
        return power_;
    }

    [[nodiscard]] double get_coefficient() const {
        return coefficient_;
    }

    explicit Variable(const double coefficient = 0, std::string variable = "x", const double power = 0) :
    coefficient_(coefficient),
    power_(power),
    variable_name_(std::move(variable)) {}

    Variable(const Variable &copied) {
        coefficient_ = copied.coefficient_;
        variable_name_ = copied.variable_name_;
        power_ = copied.power_;
    }

    explicit Variable(std::string buffer) {
        std::string temp_buffer;
        int i;
        for (i = 0; (buffer[i] > 47 && buffer[i] < 58) || buffer[i] == '-' || buffer[i] == '+'; ++i) {
            temp_buffer.push_back(buffer[i]);
        }

        if (temp_buffer.empty())
            coefficient_ = 1;
        else
            coefficient_ = std::stod(temp_buffer);

        temp_buffer.clear();

        if (buffer[i] < 'A' || (buffer[i] > 'Z' && buffer[i] < 'a') || buffer[i] > 'z')
            throw AlgebraException("Impossible input", VARIABLE_IMPOSSIBLE_NAME);

        temp_buffer = buffer[i++];
        variable_name_ = temp_buffer[0];
        temp_buffer.clear();

        if (buffer[i] == '^')
            ++i;

        if (buffer[i] == '-')
            temp_buffer.push_back(buffer[i++]);

        if (buffer[i] < 48 || buffer[i] > 57)
            throw AlgebraException("Impossible input", VARIABLE_IMPOSSIBLE_NAME);

        while (buffer[i] > 47 && buffer[i] < 58 && i < buffer.size())
            temp_buffer.push_back(buffer[i++]);

        power_ = std::stod(temp_buffer);
    }

    Variable &operator=(const Variable &source) {
        coefficient_ = source.coefficient_;
        variable_name_ = source.variable_name_;
        power_ = source.power_;
        return *this;
    }

    bool operator==(const Variable &source) const {
        return coefficient_ == source.coefficient_ && variable_name_ == source.variable_name_ && power_ == source.power_;
    }

    bool operator!=(const Variable &source) const {
        return !(*this == source);
    }

    Variable operator+() const {
        return *this;
    }

    Variable operator+(const Variable &addend) const {
        if (!check_if_summable(*this, addend))
            throw AlgebraException("Variables are not summable", VARIABLE_NOT_SUMMABLE);

        return Variable{coefficient_ + addend.coefficient_, variable_name_, power_};
    }

    Variable operator-() const {
        return Variable{- this->coefficient_, this->variable_name_, this->power_};
    }

    Variable operator-(const Variable &subtrahend) const {
        if (!check_if_summable(*this, subtrahend))
            throw AlgebraException("Variables are not summable", VARIABLE_NOT_SUMMABLE);
        return Variable{coefficient_ - subtrahend.coefficient_, variable_name_, power_};
    }

    Variable &operator+=(const Variable &addend) {
        coefficient_ = (*this + addend).get_coefficient();
        return *this;
    }

    Variable &operator-=(const Variable &subtrahend) {
        coefficient_ = (*this + subtrahend).get_coefficient();
        return *this;
    }

    Variable operator*(const double multiplier) const {
        return Variable{this->coefficient_ * multiplier, this->variable_name_, this->power_};
    }

    Variable operator*(const Variable &multiplier) const {
        if (!check_if_multipliable(*this, multiplier))
            throw AlgebraException("Variables are not multipliable", VARIABLE_NOT_MULTIPLIABLE);
        return Variable{this->coefficient_ * multiplier.coefficient_,
                              this->variable_name_,
                              this->power_ + multiplier.power_};
    }

    Variable operator/(const double divider) const {
        if (divider == 0)
            throw AlgebraException("Division by zero", VARIABLE_DIVISION_BY_ZERO);
        return Variable{this->coefficient_ / divider, this->variable_name_, this->power_};
    }

    Variable operator/(const Variable &divider) const {
        if (!check_if_divisible(*this, divider))
            throw AlgebraException("Division by zero", VARIABLE_DIVISION_BY_ZERO);
        return Variable{this->coefficient_ / divider.coefficient_,
                              this->variable_name_,
                              this->power_ - divider.power_};
    }

    Variable &operator*=(const double multiplier) {
        coefficient_ *= multiplier;
        return *this;
    }

    Variable &operator*=(const Variable &multiplier) {
        if (!check_if_multipliable(*this, multiplier))
            throw AlgebraException("Variables are not multipliable", VARIABLE_NOT_MULTIPLIABLE);
        coefficient_ *= multiplier.coefficient_;
        return *this;
    }

    Variable &operator/=(const double divider) {
        if (divider == 0)
            throw AlgebraException("Division by zero", VARIABLE_DIVISION_BY_ZERO);
        coefficient_ /= divider;
        return *this;
    }

    Variable &operator/=(const Variable &divider) {
        if (!check_if_divisible(*this, divider))
            throw AlgebraException("Division by zero", VARIABLE_DIVISION_BY_ZERO);
        coefficient_ /= divider.coefficient_;
        power_ -= divider.power_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &out, const Variable &variable) noexcept;

    friend std::istream& operator>>(std::istream &from, Variable &variable);

    ~Variable() = default;
};

std::ostream &operator<<(std::ostream &out, const Variable &variable) noexcept {
    std::string power = "^";
    std::string left = "(";
    std::string right = ")";
    std::string plus = "+";
    if (variable.coefficient_ != 0) {
        if (variable.coefficient_ > 0)
            out << plus;
        out << std::to_string(variable.coefficient_);
        if(variable.power_ != 0) {
            out << variable.variable_name_ << power;
            if (variable.power_ > 0)
                out << std::to_string(variable.power_);
            else {
                out << left << std::to_string(variable.power_) << right;
            }
        }
    }
    return out;
}

std::istream &operator>>(std::istream &from, Variable &variable) {
    std::string buffer;
    from >> buffer;

    variable = Variable(buffer);

    return from;
}

