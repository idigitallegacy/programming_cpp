//
// Created by Михаил on 16.03.2022.
//
#include <functional>
#include <utility>

#ifndef CPP_ALGORITHM_H
#define CPP_ALGORITHM_H

#endif //CPP_ALGORITHM_H


/**
 * Checks if all of the elements in range [begin; end) are satisfying some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i) for each i : [begin; end) is true. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool all_of(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    for (Iterator_t i = begin; i != end; ++i)
        if (!predicate(*i))
            return false;
    return true;
}

/**
 * Checks if at least one of the elements in range [begin; end) is satisfying some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i) for any i : [begin; end) is true. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool any_of(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    for (Iterator_t i = begin; i != end; ++i)
        if (predicate(*i))
            return true;
    return false;
}

/**
 * Checks if none of the elements in range [begin; end) are satisfying some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if !predicate(i) for each i : [begin; end) is true. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool none_of(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    return !all_of(begin, end, predicate);
}

/**
 * Checks if exactly one of the elements in range [begin; end) is satisfying some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i) for only one i : [begin; end) is true. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool one_of(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    bool counter = false;
    for (Iterator_t i = begin; i != end; ++i) {
        if (!counter && predicate(*i))
            counter = true;
        else if (counter && predicate(*i))
            return false;
    }
    return counter;
}

/**
 * Checks if  the elements in range [begin; end) are sorted by some predicate order
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 2 arguments)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i, j) for each i : [begin; end) and j : [begin; end) is true. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool is_sorted(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {

    for (Iterator_t i = begin; i != end - 1; ++i)
        if (!predicate(*i, *(i + 1)))
            return false;
    return true;
}

/**
 * Checks if the range [begin; end) is partitioned by some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i) for each i : [begin; j] and !predicate(k) for each k : (j, end). \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool is_partitioned(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    bool part = predicate(*begin);
    bool changed = false;

    for (Iterator_t i = begin + 1; i != end; ++i) {
        bool element_part = predicate(*i);
        if (element_part != part && !changed) {
            changed = true;
            part = !part;
        } else if (element_part != part)
            return false;
    }
    return changed;
}

/**
 * Finds the first element that is not equal to given in range [begin; end).
 * @tparam Iterator_t Iterator type
 * @tparam Element_t Element type (must be comparable to each element in range)
 * @param begin Range begin
 * @param end Range end
 * @param element Needle
 * @return \b Iterator to the first element not equal to given. \n
 * \b End iterator if not found.
 */
template<class Iterator_t, typename Element_t>
Iterator_t find_not(const Iterator_t &begin, const Iterator_t &end, const Element_t &element) {
    for (Iterator_t i = begin; i != end; ++i)
        if (*i != element)
            return i;
    return end;
}

/**
 * Finds the first element that is equal to given in range [begin; end) starting from the end.
 * @tparam Iterator_t Iterator type
 * @tparam Element_t Element type (must be comparable to each element in range)
 * @param begin Range begin
 * @param end Range end
 * @param element Needle
 * @return \b Iterator to the first element equal to given. \n
 * \b Begin iterator if not found.
 */
template<class Iterator_t, typename Element_t>
Iterator_t find_backward(const Iterator_t &begin, const Iterator_t &end, const Element_t &element) {
    for (Iterator_t i = end - 1; i != begin; --i)
        if (*i == element)
            return i;
    return begin; // Not found or begin is the needle
}

/**
 * Checks if the range [begin; end) is palindrome by some predicate condition
 * @tparam Iterator_t Iterator type
 * @tparam Function_t Predicate type (must be a function of 1 argument)
 * @param begin Range begin
 * @param end Range end
 * @param predicate Predicate (must be convertible to \b boolean)
 * @return \b True if predicate(i) for each i : [begin; j] and predicate(k) for each k : (j, end); k = end - i. \n
 * \b False otherwise
 */
template<class Iterator_t, class Function_t>
bool is_palindrome(const Iterator_t &begin, const Iterator_t &end, const Function_t predicate) {
    if (begin == end)
        return true;
    for (Iterator_t i = begin, j = end - 1; i != j; ++i, --j)
        if (predicate(*i) != predicate(*j))
            return false;
    return true;
}