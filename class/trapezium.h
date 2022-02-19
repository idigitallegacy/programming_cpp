//
// Created by Михаил on 09.02.2021.
//

#ifndef LABS_PROGRAMMING_TRAPEZIUM_H
#ifdef LABS_PROGRAMMING_POLYGON_H
#define LABS_PROGRAMMING_TRAPEZIUM_H

class Trapezium : public Polygon {
private:
    /// Checks if the [source] is a trapezium
    static bool check_if_trapezium(const std::vector<Point> &source) {
        if (source.size() < 4)
            throw FigureException("Too few points to initialize a trapezium", TOO_FEW_POINTS);
        if (source.size() > 4)
            throw FigureException("Too much points to initialize a trapezium", TOO_MUCH_POINTS);

        if (!check_if_convex(source))
            throw FigureException("New shape is not convex", NOT_CONVEX);

        if (!check_if_convex(source))
            return false;

        for (unsigned long long i = 0; i < source.size(); ++i) {
            if (!check_if_unique(source, i, source[i]))
                return false;
        }

        Point AB(source[1].getX() - source[0].getX(), source[1].getY() - source[0].getY());
        Point BC(source[2].getX() - source[1].getX(), source[2].getY() - source[1].getY());
        Point CD(source[3].getX() - source[2].getX(), source[3].getY() - source[2].getY());
        Point DA(source[0].getX() - source[3].getX(), source[0].getY() - source[3].getY());

        if ((AB.getX() * CD.getY() - AB.getY() * CD.getX()) != 0 &&
            (BC.getX() * DA.getY() - BC.getY() * DA.getX()) != 0)
            return false;

        if ((AB.getX() * CD.getY() - AB.getY() * CD.getX()) == 0 &&
            (AB.getX() * BC.getY() - AB.getY() * BC.getX()) == 0)
            return false;
        return true;
    }

public:
    /// Changes the coordinates of point to (new_x; new_y). Skips if the point was not found, point
    /// located at (new_x; new_y) is already exists at the polyline or changing the point comes to trapezium destroying
    void change_point(const Point &point, double new_x, double new_y) final {
        std::vector<Point> tmp = points_;
        Polygon::change_point(point, new_x, new_y);
        if (!check_if_trapezium(points_))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
    }

    void push_point(const Point &point) override {
        throw FigureException("Pushing a point destroys figure", STRUCTURE_DESTROYING);
    }

    explicit Trapezium(const Point pts[], const unsigned long long count = 0) : Polygon(pts, count) {
        if (!check_if_trapezium(points_))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
    }

    explicit Trapezium(const std::vector<Point> &pts) : Polygon(pts) {
        if (!check_if_trapezium(points_))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
    }

    explicit Trapezium(const Polygon &source) : Polygon(source) {
        if (!check_if_trapezium(points_))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
    }

#ifdef LABS_PROGRAMMING_CORRECTPOLYGON_H

    explicit Trapezium(const CorrectPolygon &source) : Polygon(source.get_polyline()) {
        if (!check_if_trapezium(points_))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
    }

#endif
    // Copy constructor
    Trapezium(const Trapezium &source) noexcept = default;

    Trapezium &operator=(const std::vector<Point> &source) {
        if (!check_if_trapezium(source))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
        points_ = source;
        return *this;
    }

#ifdef LABS_PROGRAMMING_CORRECTPOLYGON_H
    Trapezium &operator=(const CorrectPolygon &source) {
        if (!check_if_trapezium(source.get_polyline()))
            throw FigureException("New shape is not a trapezium", NOT_A_TRAPEZIUM);
        points_ = source.get_polyline();
        return *this;
    }

#endif

    Trapezium &operator=(const Trapezium &source) noexcept {
        points_ = source.points_;
        return *this;
    }
};

#endif //LABS_PROGRAMMING_POLYGON_H
#endif //LABS_PROGRAMMING_TRAPEZIUM_H
