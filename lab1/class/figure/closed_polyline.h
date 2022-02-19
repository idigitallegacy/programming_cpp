#ifndef LABS_PROGRAMMING_CLOSED_POLYLINE_H
#ifdef LABS_PROGRAMMING_POLYLINE_H
#define LABS_PROGRAMMING_CLOSED_POLYLINE_H

class ClosedPolyline : public Polyline {
protected:
    /// Checks if there are some other points equal to [needle], but not at [index] position at [source]
    static bool check_if_unique(const std::vector<Point> &source, const long long index, const Point &needle) {
        return !(std::any_of(source.begin(), source.begin() + index, equal_points_predicate(needle)) &&
                 std::any_of(source.begin() + index + 1, source.end(), equal_points_predicate(needle)));
    }

public:
    /// Returns the length of a closed polyline
    [[nodiscard]] double get_length() const noexcept final {
        double length = 0;
        for (long long i = 0; i < points_.size() - 1; ++i) {
            length += std::sqrt(
                    (points_[i + 1].getX() - points_[i].getX()) * (points_[i + 1].getX() - points_[i].getX())
                    + (points_[i + 1].getY() - points_[i].getY()) * (points_[i + 1].getY() - points_[i].getY()));
        }
        length += sqrt((points_[points_.size() - 1].getX() - points_[0].getX())
                       * (points_[points_.size() - 1].getX() - points_[0].getX())
                       + (points_[points_.size() - 1].getY() - points_[0].getY())
                         * (points_[points_.size() - 1].getY() - points_[0].getY()));
        return length;
    }

    /// This constructor is only used for dynamic polymorphism
    ClosedPolyline() noexcept : Polyline() {}

    explicit ClosedPolyline(const Point pts[], unsigned long long count = 3) : Polyline(pts, count) {}

    explicit ClosedPolyline(const std::vector<Point> &pts) : Polyline(pts) {}

    explicit ClosedPolyline(const Polyline &source) : Polyline(source) {}

    ClosedPolyline &operator=(const ClosedPolyline &source) {
        points_ = source.get_polyline();
        return *this;
    }
};

#endif //LABS_PROGRAMMING_POLYLINE_H
#endif //LABS_PROGRAMMING_CLOSED_POLYLINE_H
