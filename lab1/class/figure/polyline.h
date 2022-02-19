#ifndef LABS_PROGRAMMING_POLYLINE_H
#define LABS_PROGRAMMING_POLYLINE_H
#endif

/*
 * Exception IDs:
 * 1 - Point was not found
 * 2 - (changing point(x;y) to point(x_n;y_n)) Point located at (x_n; y_n) already exists
 */
class Polyline {
protected:
    std::vector<Point> points_;

    struct UPointsEqual { // Unary predicate for is_exists() method
        Point point_;

        explicit UPointsEqual(const Point &point) : point_(point) {}

        bool operator()(const Point &comparison) const { return comparison == point_; }
    };

    typedef UPointsEqual equal_points_predicate; // decorator

    bool is_polyline(const std::vector<Point> &source) {
        for (int i = 1; i < source.size(); ++i) {
            Point AB(source[i].getX() - source[i - 1].getX(),
                     source[i].getY() - source[i - 1].getY());
            Point BC(source[(i + 1) % source.size()].getX() - source[i].getX(),
                     source[(i + 1) % source.size()].getY() - source[i].getY());
            Point CD(source[(i + 2) % source.size()].getX() - source[(i + 1) % source.size()].getX(),
                     source[(i + 2) % source.size()].getY() - source[(i + 1) % source.size()].getY());

            double Tol2 = 1e-20; //Квадрат допустимой погрешности
            double res = (CD.getX() - AB.getX()) / (BC.getX() - AB.getX()) - (CD.getY() - AB.getY()) / (BC.getY() - AB.getY());
            if (pow(res, 2) > Tol2)
                    return true;
        }
        return false;
    }

    /// Checks if the point already exists at the polyline
    [[nodiscard]] bool is_exists(const Point &point) const noexcept {
        if (std::any_of(points_.begin(), points_.end(), equal_points_predicate(point)))
            return true;
        return false;
    }

    /// Returns the index of needle point at the array, if found. If not, returns 0 (so it should be used with
    /// is_exists() method).
    [[nodiscard]] unsigned long long get_index(const Point &point) const noexcept {
        for (unsigned long long i = 0; i < points_.size(); ++i)
            if (points_[i] == point)
                return i;
        return 0;
    }

public:
    /// Changes the coordinates of point to (new_x; new_y). Skips if the point was not found or point
    /// located at (new_x; new_y) is already exists at the polyline.
    virtual void change_point(const Point &point, const double new_x, const double new_y) {
        if (!is_exists(point))
            throw FigureException("Point not found", POINT_NOT_FOUND);
        else if (is_exists(Point(new_x, new_y))) {
            throw FigureException("Point is already exists. Use erase() method.", POINT_ALREADY_EXISTS);
        } else {
            std::vector<Point> tmp = points_;
            unsigned long long index = get_index(point);
            points_[index].setX(new_x);
            points_[index].setY(new_y);
            if (!is_polyline(points_)) {
                points_ = tmp;
                throw FigureException("Shape is not a polyline", NOT_A_POLYLINE);
            }
        }
    }

    /// Adds point if it's a unique point
    virtual void push_point(const Point &point) {
        if (is_exists(point)) {
            throw FigureException("Point is already exists.", POINT_ALREADY_EXISTS);
        }
        points_.push_back(point);
    }

    /// Erases point from polyline. Skips if the point was not found, point
    /// located at (new_x; new_y) is already exists at the polyline or deletion of a point will result in a line.
    void erase(const Point &point) {
        if (points_.size() < 4) {
            throw FigureException("Deletion of a point will destroy structure.", STRUCTURE_DESTROYING);
        }
        std::vector<Point> tmp = points_;
        unsigned long long index = get_index(point);
        if (index == 0 && !is_exists(point)) {
            throw FigureException("Point not found.", POINT_NOT_FOUND);
        }
        points_.erase(points_.begin() + index);
        if (!is_polyline(points_)) {
            points_ = tmp;
            throw FigureException("Shape is not a polyline", STRUCTURE_DESTROYING);
        }
    }

    /// Returns std::vector<Point> - an array of points at the polyline
    [[nodiscard]] std::vector<Point> get_polyline() const noexcept {
        return points_;
    }

    /// Returns a nuber of points in the polyline
    [[nodiscard]] unsigned long long get_size() const noexcept {
        return points_.size();
    }

    /// Returns length of the polyline
    [[nodiscard]] virtual double get_length() const noexcept {
        double length = 0;
        for (unsigned long long i = 0; i < points_.size() - 1; ++i) {
            length += std::sqrt(
                    (points_[i + 1].getX() - points_[i].getX()) * (points_[i + 1].getX() - points_[i].getX())
                    + (points_[i + 1].getY() - points_[i].getY()) * (points_[i + 1].getY() - points_[i].getY()));
        }
        return length;
    }

    /// This constructor is only used for dynamic polymorphism
    Polyline() noexcept {
        Point tmp_a(0, 0);
        Point tmp_b(1, 0);
        Point tmp_c(1, 1);
        std::vector<Point> array = {tmp_a, tmp_b, tmp_c};
        points_ = array;
    }

    explicit Polyline(const Point pts[], unsigned long long count = 3) {
        if (count < 2) {
            throw FigureException("Too few points to initialize polyline", TOO_FEW_POINTS);
        }
        unsigned long long added_points = 0;
        for (int i = 0; i < count; ++i)
            if (!is_exists(pts[i])) {
                points_.push_back(pts[i]);
                ++added_points;
            }
        if (added_points < 3)
            throw FigureException("Added too few points to initialize polyline", TOO_FEW_POINTS);
        if (!is_polyline(points_))
            throw FigureException("Shape is not a polyline", NOT_A_POLYLINE);
    }

    explicit Polyline(const std::vector<Point> &pts) {
        if (pts.size() < 2)
            throw FigureException("Too few points to initialize polyline", TOO_FEW_POINTS);

        unsigned long long added_points = 0;
        for (const auto &pt : pts)
            if (!is_exists(pt)) {
                points_.push_back(pt);
                ++added_points;
            }

        if (added_points < 3)
            throw FigureException("Added too few points to initialize polyline", TOO_FEW_POINTS);
        if (!is_polyline(points_))
            throw FigureException("Shape is not a polyline", NOT_A_POLYLINE);
    }

    Polyline(const Polyline &source) noexcept {
        points_ = source.points_;
    }

    Polyline &operator=(const Polyline &source) noexcept {
        points_ = source.points_;
        return *this;
    }

    ~Polyline() noexcept {
        points_.clear();
    }
};

