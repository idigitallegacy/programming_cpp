#ifndef LABS_PROGRAMMING_POINT_H
#define LABS_PROGRAMMING_POINT_H


class Point {
private:
    double x_;
    double y_;
public:
    Point() : x_(0), y_(0) {};

    Point(const double x, const double y) : x_(x), y_(y) {}

    Point(const Point &source) {
        x_ = source.x_;
        y_ = source.y_;
    }

    Point &operator=(const Point &source) = default;

    /// Sets the x coordinate of a point to new_x
    void setX(const double new_x) {
        x_ = new_x;
    }

    /// Sets the y coordinate of a point to new_y
    void setY(const double new_y) {
        y_ = new_y;
    }

    void setXY(const double new_x, const double new_y) {
        x_ = new_x;
        y_ = new_y;
    }

    /// Returns the x coordinate
    [[nodiscard]] double getX() const {
        return x_;
    }

    /// Returns the y coordinate
    [[nodiscard]] double getY() const {
        return y_;
    }

    bool operator==(const Point &source) const {
        return this->getX() == source.getX() && this->getY() == source.getY();
    }

    bool operator!=(const Point &source) const {
        return !(this->getX() == source.getX() && this->getY() == source.getY());
    }

    ~Point() = default;
};


#endif //LABS_PROGRAMMING_POINT_H
