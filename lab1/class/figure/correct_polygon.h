//
// Created by Михаил on 09.02.2021.
//

#ifndef LABS_PROGRAMMING_CORRECTPOLYGON_H
#ifdef LABS_PROGRAMMING_POLYGON_H

#define LABS_PROGRAMMING_CORRECTPOLYGON_H

class CorrectPolygon : public Polygon {
private:
    /// Checks if [source] is a correct polygon
    static bool check_if_correct(const std::vector<Point> &source) {
        if (source.size() < 3)
            throw FigureException("Too few points to initialize a polygon", TOO_FEW_POINTS);

        if (!check_if_convex(source))
            throw FigureException("New shape is not convex", NOT_CONVEX);

        for (long long i = 0; i < source.size(); ++i) {
            if (!check_if_unique(source, i, source[i]))
                return false;
        }

        double length = 0;
        for (int i = 0; i < source.size(); ++i) {
            double tmp = std::sqrt((source[i + 1].getX() - source[i].getX())
                                   * (source[i + 1].getX() - source[i].getX())
                                   + (source[i + 1].getY() - source[i].getY())
                                     * (source[i + 1].getY() - source[i].getY()));
            if (i == 0) {
                length = tmp;
                continue;
            }
            if (length != tmp) return false;
        }
        return true;
    }

public:
    /// Changing point comes to incorrect polygon, so it's prohibited
    void change_point(const Point &source, const double new_x, const double new_y) override {
        throw FigureException("Changing points at the correct polygon is not allowed", CHANGING_POINT_IS_NOT_ALLOWED);
    };

    /// Adds a multiplier to points coordinates. E.g. Point(2;3) after CorrectPolygon.reshape(2) becomes (4;6)
    void reshape(const double multiplier) {
        if (multiplier == 0)
            throw FigureException("Invalid point multiplyer == 0", INVALID_POINT_MULTIPLYER);
        for (auto &i : points_) {
            i.setX(i.getX() * multiplier);
            i.setY(i.getY() * multiplier);
        }
    }

    explicit CorrectPolygon(const Point pts[], const unsigned long long count = 0) : Polygon(pts, count) {
        if (!check_if_correct(points_))
            throw FigureException("New polygon is arbitrary.", ARBITRARY_POLYGON);
    }

    explicit CorrectPolygon(const std::vector<Point> &pts) : Polygon(pts) {
        if (!check_if_correct(pts))
            throw FigureException("New polygon is arbitrary.", ARBITRARY_POLYGON);
    }

    explicit CorrectPolygon(const Polygon &source) : Polygon(source) {
        if (!check_if_correct(source.get_polyline()))
            throw FigureException("New polygon is arbitrary.", ARBITRARY_POLYGON);
    }

    // Copy constructor
    CorrectPolygon(const CorrectPolygon &source) noexcept : Polygon(source.get_polyline()) {
        points_ = source.points_;
    }

    CorrectPolygon &operator=(const CorrectPolygon &source) {
        if (!check_if_correct(source.get_polyline()))
            throw FigureException("New polygon is arbitrary.", ARBITRARY_POLYGON);
        points_ = source.get_polyline();
        return *this;
    }

    ~CorrectPolygon() = default;
};

#endif //LABS_PROGRAMMING_POLYGON_H
#endif //LABS_PROGRAMMING_CORRECTPOLYGON_H
