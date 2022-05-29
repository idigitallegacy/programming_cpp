//
// Created by Михаил on 19.02.2022.
//

#include <pthread.h>
#include <gtest/gtest.h>
#include "classes/iterator.h"
#include "classes/circle_buffer.h"
#include "algorithm.h"
#include "classes/double_queue.h"


bool predicate_div2(const int el) {
    return !(el % 2);
}

bool predicate_issorted(const int l, const int r) {
    return l < r;
}

/**
 * All of the test cases uses _CircleBuffer_ Iterator, so I intentionally use capacity == 6 instead of capacity == 5
 * to avoid equality of the end pointer to container[0].
 */

TEST(CircleBuffer, Init) { // Just Circle buffer initialization
    EXPECT_NO_THROW(CircleBuffer<int> a); // init
    EXPECT_NO_THROW(CircleBuffer<double> b(10)); // init with capacity
    CircleBuffer<int> a;
    CircleBuffer<double> b(10);

    EXPECT_EQ(a.capacity(), 1); // Default capacity is at least one element
    EXPECT_EQ(a.size(), 0);

    EXPECT_EQ(b.capacity(), 10);
    EXPECT_EQ(b.size(), 0); // No elements added
}

TEST(CircleBuffer, OneElementAddition) {
    CircleBuffer<int> a(10);
    a.emplace_back(10);
    EXPECT_EQ(a.get_first(), 10);
    EXPECT_EQ(a.get_last(), 10);
}

TEST(CircleBuffer, MultipleElementAddition__WithoutOverflow) { // Use Circle buffer as an array
    CircleBuffer<int> a(10);
    a.emplace_back(10);
    a.emplace_back(20);
    a.emplace_back(30);
    EXPECT_EQ(a.get_first(), 10);
    EXPECT_EQ(a.get_last(), 30);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[10], 10);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.empty(), false);

    a.remove_first();
    EXPECT_EQ(a.get_first(), 20);
    EXPECT_EQ(a.get_last(), 30);

    a.remove_last();
    EXPECT_EQ(a.get_first(), 20);
    EXPECT_EQ(a.get_last(), 20);

    a.remove_first();
    EXPECT_THROW(a.get_first(), CircleBufferException);
    EXPECT_THROW(a.get_last(), CircleBufferException);
    try {
        a.get_last();
    } catch (CircleBufferException &exception) {
        EXPECT_EQ(exception.get_id(), CircleBufferException::BUFFER_EMPTY);
    }
}

TEST(CircleBuffer, MultipleElementAddition__WithOverflow) { // Use Circle buffer with overflow (el_number % capacity)
    CircleBuffer<int> a(5);
    a.emplace_back(10);
    a.emplace_back(20);
    a.emplace_back(30);
    a.emplace_back(40);
    a.emplace_back(50);
    a.emplace_back(60);
    EXPECT_EQ(a.get_first(), 20);
    EXPECT_EQ(a.get_last(), 60);

    a.remove_last();
    EXPECT_EQ(a.get_last(), 50);

    a.remove_first();
    EXPECT_EQ(a.get_first(), 30);

    a.emplace_first(60);
    EXPECT_EQ(a.get_first(), 60);
    EXPECT_EQ(a.get_last(), 50);

    a.remove_first();
    a.remove_first();
    EXPECT_EQ(a.get_first(), 40);

    a.remove_last();
    a.remove_last();
    EXPECT_THROW(a.remove_last(), CircleBufferException);
    try {
        a.get_last();
    } catch (CircleBufferException &exception) {
        EXPECT_EQ(exception.get_id(), CircleBufferException::BUFFER_EMPTY);
    }

    a.emplace_back(10);
    a.emplace_back(20);
    a.emplace_back(30);
    a.emplace_back(40);
    EXPECT_EQ(a.get_first(), 10);
    EXPECT_EQ(a.get_last(), 40);

    a.emplace_back(50);
    a.emplace_back(60);
    EXPECT_EQ(a.get_last(), 60);
    EXPECT_EQ(a.get_first(), 20);

    a.emplace_first(70);
    EXPECT_EQ(a.get_first(), 70);
    EXPECT_EQ(a.get_last(), 50);
}

TEST(Algorithm, AllOf) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 6;
    a[3] = 8;
    a[4] = 10;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(all_of<Iterator<int>>(begin, end, predicate_div2), true);
    EXPECT_EQ(all_of<Iterator<int>>(begin, end, [](int el) {return predicate_div2(el);}), true);
    a[4] = 9;
    EXPECT_EQ(all_of<Iterator<int>>(begin, end, predicate_div2), false);
    delete [] a;
}

TEST(Algorithm, AnyOf) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 6;
    a[3] = 8;
    a[4] = 10;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(any_of<Iterator<int>>(begin, end, &predicate_div2), true);

    a[0] = 1;
    a[1] = 3;
    a[2] = 5;
    a[3] = 7;
    a[4] = 9;
    EXPECT_EQ(any_of<Iterator<int>>(begin, end, &predicate_div2), false);
    delete [] a;
}

TEST(Algorithm, NoneOf) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 6;
    a[3] = 8;
    a[4] = 10;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(none_of<Iterator<int>>(begin, end, &predicate_div2), false);

    a[0] = 1;
    a[1] = 3;
    a[2] = 5;
    a[3] = 7;
    a[4] = 9;
    EXPECT_EQ(none_of<Iterator<int>>(begin, end, &predicate_div2), true);
    delete [] a;
}

TEST(Algorithm, OneOf) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 6;
    a[3] = 8;
    a[4] = 10;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(one_of<Iterator<int>>(begin, end, &predicate_div2), false);

    a[0] = 1;
    a[1] = 3;
    a[2] = 5;
    a[3] = 7;
    a[4] = 9;
    EXPECT_EQ(one_of<Iterator<int>>(begin, end, &predicate_div2), false);

    a[0] = 1;
    a[1] = 2;
    a[2] = 5;
    a[3] = 7;
    a[4] = 9;
    EXPECT_EQ(one_of<Iterator<int>>(begin, end, &predicate_div2), true);
    delete [] a;
}

TEST(Algorithm, IsSorted) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 6;
    a[3] = 8;
    a[4] = 10;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(is_sorted<Iterator<int>>(begin, end, &predicate_issorted), true);

    a[0] = 5;
    EXPECT_EQ(is_sorted<Iterator<int>>(begin, end, &predicate_issorted), false);

    a[1] = 4;
    a[2] = 3;
    a[3] = 2;
    a[4] = 1;
    EXPECT_EQ(is_sorted<Iterator<int>>(begin, end, &predicate_issorted), false);
    delete [] a;
}

TEST(Algorithm, IsPartitioned) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 4;
    a[2] = 5;
    a[3] = 7;
    a[4] = 9;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(is_partitioned<Iterator<int>>(begin, end, &predicate_div2), true);

    a[0] = 5;
    EXPECT_EQ(is_partitioned<Iterator<int>>(begin, end, &predicate_div2), false);

    a[1] = 4;
    a[2] = 3;
    a[3] = 2;
    a[4] = 1;
    EXPECT_EQ(is_partitioned<Iterator<int>>(begin, end, &predicate_div2), false);
    delete [] a;
}

TEST(Algorithm, FindNot) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 2;
    a[2] = 2;
    a[3] = 2;
    a[4] = 4;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(find_not<Iterator<int>>(begin, end, 2), Iterator<int>(a, a + 4, 6));

    a[4] = 2;
    EXPECT_EQ(find_not<Iterator<int>>(begin, end, 2), end);
    delete [] a;
}

TEST(Algorithm, FindBackward) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 2;
    a[2] = 2;
    a[3] = 2;
    a[4] = 4;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(find_backward<Iterator<int>>(begin, end, 2), Iterator<int>(a, a + 3, 6));

    a[4] = 2;
    EXPECT_EQ(find_backward<Iterator<int>>(begin, end, 4), begin);
    delete [] a;
}

TEST(Algorithm, IsPalindrome) {
    int *a = new int[5];
    a[0] = 2;
    a[1] = 2;
    a[2] = 2;
    a[3] = 2;
    a[4] = 2;
    Iterator<int> begin(a, a, 6);
    Iterator<int> end(a, a + 5, 6);
    EXPECT_EQ(is_palindrome<Iterator<int>>(begin, end, &predicate_div2), true);

    a[4] = 5;
    EXPECT_EQ(is_palindrome<Iterator<int>>(begin, end, &predicate_div2), false);

    a[0] = 1;
    EXPECT_EQ(is_palindrome<Iterator<int>>(begin, end, &predicate_div2), true);

    delete [] a;
}

TEST(Double_queue, Init) {
    EXPECT_NO_THROW(DoubleQueue<int> a);
    EXPECT_NO_THROW(DoubleQueue<int> a({1, 2, 3, 4, 5}));
}

TEST(Double_queue, SizeLimit) {
    std::vector<int> init(1e6, 0);
    for (long long i = 0; i < 1e6; ++i) {
        init[i] = (int) i;
    }
    DoubleQueue<int> a(init);
    EXPECT_NO_THROW(a.push_back(1e6));
    for (long long i = 0; i < 1e6 + 1; ++i) {
        EXPECT_NO_THROW(a.top());
        EXPECT_EQ(a.top(), i);
        a.pop_first();
    }
}

TEST(Double_queue, Forward_addition) {
    DoubleQueue<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    EXPECT_EQ(a.top(), 1);
    a.pop_first();
    EXPECT_EQ(a.top(), 2);
    a.pop_first();
    EXPECT_EQ(a.top(), 3);
    a.pop_first();
    EXPECT_THROW(a.pop_first(), DQException);
}

TEST(Double_queue, Backward_addition) {
    DoubleQueue<int> a;
    a.push_first(1);
    a.push_first(2);
    a.push_first(3);
    EXPECT_EQ(a.top(), 3);
    a.pop_first();
    EXPECT_EQ(a.top(), 2);
    a.pop_first();
    EXPECT_EQ(a.top(), 1);
    a.pop_first();
    EXPECT_THROW(a.pop_first(), DQException);
}

TEST(Double_queue, Mixed_addition) {
    DoubleQueue<int> a;
    a.push_first(1);
    a.push_first(2);
    a.push_first(3);
    a.push_back(4);
    a.push_back(5);
    a.push_back(6);
    EXPECT_EQ(a.top(), 3);
    EXPECT_EQ(a.last(), 6);
    a.pop_first();
    a.pop_back();
    EXPECT_EQ(a.top(), 2);
    EXPECT_EQ(a.last(), 5);
    a.pop_first();
    a.pop_back();
    EXPECT_EQ(a.top(), 1);
    EXPECT_EQ(a.last(), 4);
    a.pop_first();
    a.pop_back();
    EXPECT_THROW(a.pop_first(), DQException);
    EXPECT_THROW(a.pop_back(), DQException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}