#include <utility>

//
// Created by Михаил on 18.02.2022.
//

#ifndef LAB1_POLYNOM_H
#define LAB1_POLYNOM_H

#endif //LAB1_POLYNOM_H

class Polynom {
private:
    std::vector<Variable> polynom_;

    /// Comparing variable powers
    struct {
        bool operator()(const Variable &left, const Variable &right) {
            return left.get_power() > right.get_power();
        }
    } comparison_powers_predicate;

    /// Sorts variables by power && simplifying polynom
    void sort_by_powers() {
        std::sort(polynom_.begin(), polynom_.end(), comparison_powers_predicate);
        for (long long i = 1; i < polynom_.size(); ++i) {
            if (polynom_[i].get_coefficient() == 0)
                polynom_.erase(polynom_.begin() + i);
            if (polynom_[i - 1].get_name() == polynom_[i].get_name() &&
                polynom_[i - 1].get_power() == polynom_[i].get_power()) {
                polynom_[i] += polynom_[i - 1];
                polynom_.erase(polynom_.begin() + i - 1);
            }
        }
    }

public:
    explicit Polynom(const Variable &variable) : polynom_({variable}) {}

    explicit Polynom(std::vector<Variable> polynom) : polynom_(std::move(polynom)) {}

    Polynom(const Polynom &polynom) {
        polynom_ = polynom.polynom_;
    }

    /// Returns polynom
    std::vector<Variable> get_polynom() {
        sort_by_powers();
        return polynom_;
    }

    /// Counts every variable in polynom as [variable_values].first
    [[nodiscard]] double get_value(const std::vector<std::pair<std::string, double>> &variable_values) {
        double result = 0;
        for (auto &i : polynom_) {
            for (long long j = 0; j < variable_values.size(); ++j) {
                if (i.get_name() == variable_values[j].first)
                    result += i.get_coefficient() * pow(variable_values[j].second, i.get_power());
                else if (j == variable_values.size() - 1)
                    throw AlgebraException("One of the variables is not provided.", POLYNOM_UNDEFINED_VALUE);
            }
        }
        return result;
    }

    Polynom &operator=(const double variable) {
        polynom_.emplace_back(variable);
        return *this;
    }

    Polynom &operator=(const Variable &variable) {
        polynom_.clear();
        polynom_.emplace_back(variable);
        return *this;
    }

    Polynom &operator=(const Polynom &polynom) {
        polynom_ = polynom.polynom_;
        sort_by_powers();
        return *this;
    }

    bool operator==(const Polynom &polynom) const {
        for (auto &i : polynom_) {
            if (std::find(polynom.polynom_.begin(), polynom.polynom_.end(), i) == polynom.polynom_.end())
                return false;
        }
        return true;
    };

    bool operator!=(const Polynom &polynom) const {
        return !(*this == polynom);
    }

    Polynom &operator+() {
        sort_by_powers();
        return *this;
    }

    Polynom &operator-() {
        for (auto &i : polynom_) {
            i = -i;
        }
        sort_by_powers();
        return *this;
    }

    Polynom operator+(const Polynom &polynom) const {
        std::vector<Variable> tmp = polynom_;
        for (auto &i : polynom.polynom_) {
            auto index = std::find(tmp.begin(), tmp.end(), i);
            if (index == polynom_.end())
                tmp.push_back(i);
            else
                tmp[std::distance(tmp.begin(), index)] += i;
        }
        Polynom temp_sorted(tmp);
        temp_sorted.sort_by_powers();
        return Polynom{temp_sorted};
    }

    Polynom operator+(const Variable &variable) const {
        std::vector<Variable> tmp = polynom_;
        auto index = std::find(tmp.begin(), tmp.end(), variable);
        if (index == tmp.end())
            tmp.push_back(variable);
        else
            tmp[std::distance(tmp.begin(), index)] += variable;

        Polynom temp_sorted(tmp);
        temp_sorted.sort_by_powers();
        return temp_sorted;
    }

    Polynom operator+(const double addend) const {
        Variable tmp(addend);
        return Polynom{*this + tmp};
    }

    Polynom &operator+=(const Polynom &polynom) {
        for (auto &i : polynom.polynom_) {
            auto index = std::find(polynom_.begin(), polynom_.end(), i);
            if (index == polynom_.end())
                polynom_.push_back(i);
            else
                polynom_[std::distance(polynom_.begin(), index)] += i;
        }
        sort_by_powers();
        return *this;
    }

    Polynom &operator+=(const Variable &variable) {
        auto index = std::find(polynom_.begin(), polynom_.end(), variable);
        if (index == polynom_.end())
            polynom_.push_back(variable);
        else
            polynom_[std::distance(polynom_.begin(), index)] += variable;
        sort_by_powers();
        return *this;
    }

    Polynom &operator+=(const double addend) {
        Variable tmp{addend};
        auto index = std::find(polynom_.begin(), polynom_.end(), tmp);
        if (index == polynom_.end())
            polynom_.emplace_back(addend);
        else
            polynom_[std::distance(polynom_.begin(), index)] += tmp;
        sort_by_powers();
        return *this;
    }

    Polynom operator-() const {
        Polynom tmp(polynom_);
        for (auto &i : tmp.polynom_)
            i = -i;
        return tmp;
    }

    Polynom operator-(const Polynom &polynom) const {
        return *this + (-polynom);
    }

    Polynom operator-(const Variable &variable) const {
        return *this + (-variable);
    }

    Polynom operator-(const double subtrahend) const {
        return Polynom{*this - Variable{subtrahend}};
    }

    Polynom &operator-=(const Polynom &polynom) {
        for (auto &i : polynom.polynom_) {
            auto index = std::find(polynom_.begin(), polynom_.end(), i);
            if (index == polynom_.end())
                polynom_.push_back(-i);
            else
                polynom_[std::distance(polynom_.begin(), index)] -= i;
        }
        sort_by_powers();
        return *this;
    }

    Polynom &operator-=(const Variable &variable) {
        auto index = std::find(polynom_.begin(), polynom_.end(), variable);
        if (index == polynom_.end())
            polynom_.push_back(-variable);
        else
            polynom_[std::distance(polynom_.begin(), index)] -= variable;
        sort_by_powers();
        return *this;
    }

    Polynom &operator-=(const double subtrahend) {
        Variable tmp{subtrahend};
        auto index = std::find(polynom_.begin(), polynom_.end(), tmp);
        if (index == polynom_.end())
            polynom_.emplace_back(-subtrahend);
        else
            polynom_[std::distance(polynom_.begin(), index)] -= tmp;
        sort_by_powers();
        return *this;
    }

    Polynom operator*(const double multiplier) const {
        std::vector<Variable> tmp = polynom_;
        for (auto &i : tmp)
            i *= multiplier;
        Polynom tmp_sorted(tmp);
        tmp_sorted.sort_by_powers();
        return tmp_sorted;
    }

    Polynom operator*(const Variable &multiplier) const {
        std::vector<Variable> tmp = polynom_;
        for (auto &i : tmp)
            if (multiplier.get_name() == i.get_name())
                i *= multiplier;
        Polynom tmp_sorted(tmp);
        tmp_sorted.sort_by_powers();
        return tmp_sorted;
    }

    Polynom operator*(const Polynom &polynom) const {
        std::vector<Variable> tmp;
        for (auto &i : polynom_) {
            for (auto &j : polynom.polynom_) {
                if (i.get_name() == j.get_name())
                    tmp.emplace_back(i * j);
            }
        }
        Polynom temp_sorted(tmp);
        temp_sorted.sort_by_powers();
        return temp_sorted;
    }

    Polynom operator/(const double divider) const {
        std::vector<Variable> tmp;
        if (divider == 0)
            throw AlgebraException("Division by zero", VARIABLE_DIVISION_BY_ZERO);
        for (auto &i : polynom_) {
            tmp.emplace_back(i / divider);
        }
        Polynom temp_sorted(tmp);
        temp_sorted.sort_by_powers();
        return temp_sorted;
    }

    Polynom &operator*=(const double multiplyer) {
        for (auto &i : polynom_) {
            i *= multiplyer;
        }
        sort_by_powers();
        return *this;
    }

    Polynom &operator*=(const Variable &variable) {
        for (auto &i : polynom_) {
                if (i.get_name() == variable.get_name())
                    i *= variable;
        }
        sort_by_powers();
        return *this;
    }

    Polynom &operator*=(const Polynom &polynom) {
        for (auto &i : polynom_) {
            for (auto &j : polynom.polynom_) {
                if (i.get_name() == j.get_name())
                    i *= j;
            }
        }
        sort_by_powers();
        return *this;
    }

    Polynom &operator/=(const double divider) {
        for (auto &i : polynom_) {
            i /= divider;
        }
        sort_by_powers();
        return *this;
    }

    Variable operator[](const long long index) const {
        return polynom_[index];
    }

    friend std::ostream &operator<<(std::ostream &to, Polynom &polynom);

    friend std::istream &operator>>(std::istream &from, Polynom &polynom);
};

std::ostream &operator<<(std::ostream &to, Polynom &polynom) {
    polynom.sort_by_powers();
    std::string space = " ";
    for (auto &i : polynom.polynom_) {
        to << i << space;
    }
    return to;
}

std::istream &operator>>(std::istream &from, Polynom &polynom) {
    std::string buffer;
    std::string temp_buffer;
    from >> buffer;

    for (char i : buffer) {
        if (i == '\\') {
            try {
                if (temp_buffer[0] == '+')
                    temp_buffer.erase(temp_buffer.begin());
                Variable tmp(temp_buffer);
                polynom.polynom_.push_back(tmp);
                temp_buffer.clear();
                continue;
            } catch (AlgebraException &exception) {
                continue;
            }
        }
        temp_buffer.push_back(i);
    }

    return from;
}

