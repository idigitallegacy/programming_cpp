#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

template<class T>
class RingBuffer {
private:
    int size = 0, capacity = 0;
    T *arr = new T[size];
    T *first, *last, *act_first, *act_last;
public:
    class Iterator : public iterator<random_access_iterator_tag, T> {
    private:
        T *value;
    public:
        //конструктор
        Iterator() = default;

        explicit Iterator(T *value) {
            this->value = value;
        }

        [[nodiscard]] T *get_value() const {
            return this->value;
        }

        //Операторы
        Iterator operator+(int num) {
            this->value += num;
            return *this;
        }

        Iterator operator-(int num) {
            this->value -= num;
            return *this;
        }

        Iterator operator++() {
            this->value++;
            return *this;
        }

        Iterator operator--() {
            this->value--;
            return *this;
        }

        bool operator>(const Iterator &other) {
            return this->value > other.value;
        }

        bool operator<(const Iterator &other) {
            return this->value < other.value;
        }

        bool operator>=(const Iterator &other) {
            return this->value >= other.value;
        }

        bool operator<=(const Iterator &other) {
            return this->value <= other.value;
        }

        bool operator==(const Iterator &other) {
            if (this->value == other.value)
                return true;
            return false;
        }

        bool operator!=(const Iterator &other) {
            if (this->value != other.value)
                return true;
            return false;
        }

        Iterator &operator=(T *num) {
            this->value = num;
            return *this;
        }

        Iterator &operator*() const {
            return *value;
        }

        Iterator *operator->() const {
            return &(operator*());
        }
    };

    //конструктор
    RingBuffer() = default;

    explicit RingBuffer(int size) {
        this->size = size;
        this->capacity = size + 0.5 * size;
        arr = new T[size];
        this->first = &arr[0];
        this->last = &arr[size - 1];
        this->act_first = &arr[0];
        this->act_last = &arr[0];
    }

    //конструктор копирования
    RingBuffer(const RingBuffer &other) {
        this->size = other.size;
        this->arr = other.arr;
        this->capacity = other.capacity;
    }

    //Деструктор
    ~RingBuffer() {
        delete [] arr;
    }

    Iterator iterator;

    Iterator begin() {
        return iterator(arr);
    }

    Iterator end() {
        return iterator(arr + size - 1);
    }

    void push_back(const T &el) {
        *act_last = el;
        if (last == act_last)
            act_last = first;
        else
            ++act_last;
    }

    void push_front(const T &el) {
        *act_first = el;
        if (first == act_first)
            act_first = last;
        else
            ++act_first;
    }

    void pop_back() {
        *act_last = T();
        if (act_last == first)
            act_last = last;
        else
            --act_last;
    }

    void pop_front() {
        *act_first = T();
        if (act_first == last)
            act_first = first;
        else
            ++act_first;
    }

    T operator[](int i) const {
        return this->arr[i % capacity];
    }

    friend ostream &operator<<(ostream &cout_, const RingBuffer &other) {
        for (auto i = &other.arr[0]; i != &other.arr[other.size]; ++i)
            cout << *i << " ";
        cout << endl;
        return cout_;
    }
};

class Comparison_Marks {
private:
    int rus_mark = 0;
    char eng_mark = 'F';
    string rez = "BAD";
public:
    //конструкторы
    Comparison_Marks() = default;

    Comparison_Marks(int rus_mark, char eng_mark, string rez) {
        this->rus_mark = rus_mark;
        this->eng_mark = eng_mark;
        this->rez = std::move(rez);
    }

    //конструктор копирования
    Comparison_Marks(const Comparison_Marks &other) {
        this->rus_mark = other.rus_mark;
        this->eng_mark = other.eng_mark;
        this->rez = other.rez;
    }

    //геттеры
    [[nodiscard]] int get_rus_mark() const {
        return this->rus_mark;
    }

    [[nodiscard]] char get_eng_mark() const {
        return this->eng_mark;
    }

    [[nodiscard]] string get_rez() const {
        return this->rez;
    }

    //сеттеры
    void set_rus_mark(int _rus_mark) {
        this->rus_mark = _rus_mark;
    }

    void set_eng_mark(char _eng_mark) {
        this->eng_mark = _eng_mark;
    }

    void set_rez(string _rez) {
        this->rez = std::move(_rez);
    }

    friend ostream &operator<<(ostream &cout_, const Comparison_Marks &other) {
        cout << other.rus_mark << " " << other.eng_mark << " " << other.rez << "   ";
        return cout_;
    }
};

int main() {
    //целые числа
    RingBuffer<int> buf_1(4);
    buf_1.push_back(1);
    buf_1.push_back(2);
    buf_1.push_back(3);
    buf_1.push_front(4);
    buf_1.pop_front();
    buf_1.pop_back();
    cout << "INT:" << endl << buf_1 << endl << endl;

    //строка
    RingBuffer<char> buf_2(4);
    buf_2.push_back('g');
    buf_2.push_back('b');
    buf_2.push_front('a');
    buf_2.push_back('z');
    buf_2.push_back('m');
    buf_2.push_front('s');
    buf_2.pop_front();
    buf_2.pop_back();
    cout << "CHAR:" << endl << buf_2 << endl << endl;

    //Comparison_Marks
    RingBuffer<Comparison_Marks> buf_3(3);
    buf_3.push_back({2, 'F', "BAD"});
    buf_3.push_back({3, 'C', "SO-SO"});
    buf_3.push_front({3, 'D', "SO"});
    buf_3.push_back({4, 'B', "GOOD"});
    buf_3.pop_front();
    buf_3.pop_back();
    cout << "MyClass:" << buf_3 << endl << endl << endl;

    return 0;
}