#ifndef LABS_PROGRAMMING_POLYGON_H
#ifdef LABS_PROGRAMMING_POINT_H

#define LABS_PROGRAMMING_POLYGON_H

class Polygon : public ClosedPolyline {
protected:
    /// Checks if an array of points is convex
    static bool check_if_convex(const std::vector<Point> &source) noexcept {
        double direction = 0;
        for (int i = 1; i < source.size(); ++i) {
            Point AB(source[i].getX() - source[i - 1].getX(),
                     source[i].getY() - source[i - 1].getY());
            Point BC(source[(i + 1) % source.size()].getX() - source[i].getX(),
                     source[(i + 1) % source.size()].getY() - source[i].getY());


            if (i == 1) {
                direction = AB.getX() * BC.getY() - AB.getY() * BC.getX();
                continue;
            }
            double tmp = AB.getX() * BC.getY() - AB.getY() * BC.getX();
            if (direction < 0 && tmp > 0 || direction > 0 && tmp < 0)
                return false;
            direction = tmp;
        }

        double square = 0;
        for (unsigned long long i = 0; i < source.size() - 1; ++i) {
            square += source[i].getX() * source[i + 1].getY() - source[i + 1].getX() * source[i].getY();
        }
        square += source[source.size() - 1].getX() * source[0].getY() -
                  source[0].getX() * source[source.size() - 1].getY();
        if (std::abs((float) square) == 0) return false;
        return true;
    }

public:
    /// Returns a square of a polygon
    [[nodiscard]] virtual double get_square() const noexcept {
        if (points_.size() < 3) return 0;
        double square = 0;
        for (unsigned long long i = 0; i < points_.size() - 1; ++i) {
            square += points_[i].getX() * points_[i + 1].getY() - points_[i + 1].getX() * points_[i].getY();
        }
        square += points_[points_.size() - 1].getX() * points_[0].getY() -
                  points_[0].getX() * points_[points_.size() - 1].getY();
        return std::abs(square) / 2;
    }

    /// Returns a perimeter of a polygon
    [[nodiscard]] double get_perimeter() const noexcept {
        return get_length();
    }

    /// Changes the coordinates of point to (new_x; new_y). Skips if the point was not found, point
    /// located at (new_x; new_y) is already exists at the polyline or changing the point comes to unconvex polygon.
    void change_point(const Point &point, double new_x, double new_y) override {
        if (!is_exists(point))
            throw FigureException("Point not found", POINT_NOT_FOUND);
        else if (is_exists(Point(new_x, new_y))) {
            throw FigureException("Point is already exists. Use erase() method.", POINT_ALREADY_EXISTS);
        } else {
            std::vector<Point> tmp = points_;
            ClosedPolyline::change_point(point, new_x, new_y);
            if (!check_if_convex(points_)) {
                points_ = tmp;
                throw FigureException("New figure is not convex", NOT_CONVEX);
            }
        }
    }

    /// Adds point if it's a unique point and a polygon stays convex
    void push_point(const Point &point) override {
        if (is_exists(point))
            throw FigureException("Point is already exists", POINT_ALREADY_EXISTS);
        else {
            std::vector<Point> tmp = points_;
            points_.push_back(point);
            if (!check_if_convex(points_)) {
                points_ = tmp;
                throw FigureException("New polygon is not convex.", NOT_CONVEX);
            }
        }
    }

    explicit Polygon(const Point source[], const unsigned long long count = 3) : ClosedPolyline(source, count) {
        if (!check_if_convex(points_) || count < 3)
            throw FigureException("New polygon is not convex.", NOT_CONVEX);
    }

    explicit Polygon(const std::vector<Point> &source) : ClosedPolyline(source) {
        if (source.size() < 3)
            throw FigureException("Too few points to initialize polygon.", TOO_FEW_POINTS);
        if (!check_if_convex(points_))
            throw FigureException("New polygon is not convex.", NOT_CONVEX);
    }

    explicit Polygon(const ClosedPolyline &source) : ClosedPolyline(source) {
        if (source.get_size() < 3)
            throw FigureException("Too few points to initialize polygon.", TOO_FEW_POINTS);
        if (!check_if_convex(points_))
            throw FigureException("New polygon is not convex.", NOT_CONVEX);
    }

    /// This constructor is only used for dynamic polymorphism
    Polygon() : ClosedPolyline() {
        if (!check_if_convex(points_))
            throw FigureException("Shape is not convex", NOT_CONVEX);
    }

    Polygon(const Polygon &source) : ClosedPolyline(source.points_) {};

    Polygon &operator=(const Polygon &source) {
        points_ = source.points_;
        return *this;
    }
};

#endif //LABS_PROGRAMMING_POINT_H
#endif //LABS_PROGRAMMING_POLYGON_H
