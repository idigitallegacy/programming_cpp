//
// Created by Михаил on 09.02.2021.
//

#ifndef LABS_PROGRAMMING_TRIANGLE_H
#ifdef LABS_PROGRAMMING_POLYGON_H
#define LABS_PROGRAMMING_TRIANGLE_H

class Triangle : public Polygon {
private:
    /// Checks if [source] is a triangle.
    static bool check_if_triangle(const std::vector<Point> &source) {
        if (source.size() < 3)
            throw FigureException("Too few points to initialize a triangle", TOO_FEW_POINTS);
        if (source.size() > 3)
            throw FigureException("Too much points to initialize a triangle", TOO_MUCH_POINTS);

        if (!check_if_convex(source))
            throw FigureException("New shape is not convex", NOT_CONVEX);

        for (long long i = 0; i < source.size(); ++i) {
            if (!check_if_unique(source, i, source[i]))
                return false;
        }
        return true;
    }

public:
    /// Changes the coordinates of point to (new_x; new_y). Skips if the point was not found, point
    /// located at (new_x; new_y) is already exists at the polyline or changing the point destroys the triangle.
    void change_point(const Point &point, const double new_x, const double new_y) final {
        std::vector<Point> tmp = points_;
        Polygon::change_point(point, new_x, new_y);
        if (!check_if_triangle(points_)) {
            points_ = tmp;
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
        }
    }

    void push_point(const Point &point) override {
        throw FigureException("Pushing a point destroys figure", STRUCTURE_DESTROYING);
    }

    explicit Triangle(const Point pts[], const unsigned long long count = 3) : Polygon(pts, count) {
        if (!check_if_triangle(points_))
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
    }

    explicit Triangle(const std::vector<Point> &pts) : Polygon(pts) {
        if (!check_if_triangle(pts))
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
    }

    explicit Triangle(const Polygon &source) : Polygon(source) {
        if (!check_if_triangle(source.get_polyline()))
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
        points_ = source.get_polyline();
    }

#ifdef LABS_PROGRAMMING_CORRECTPOLYGON_H

    explicit Triangle(const CorrectPolygon &source) : Polygon(source.get_polyline()) {
        if (!check_if_triangle(points_))
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
    }

#endif

    Triangle(const Triangle &source) noexcept = default;

    Triangle &operator=(const std::vector<Point> &source) {
        if (!check_if_triangle(source))
            throw FigureException("New figure is not a triangle.", NOT_A_TRIANGLE);
        points_ = source;
        return *this;
    }

    Triangle &operator=(const Triangle &source) noexcept {
        points_ = source.points_;
        return *this;
    }
};

#endif //LABS_PROGRAMMING_POLYGON_H
#endif //LABS_PROGRAMMING_TRIANGLE_H
