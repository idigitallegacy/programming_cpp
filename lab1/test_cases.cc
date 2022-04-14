#include <gtest/gtest.h>

#include "linker/linker.h"


TEST(Point, Creation) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(1.1, -1);

    EXPECT_EQ(a.getX(), 0);
    EXPECT_EQ(a.getY(), 0);

    EXPECT_EQ(b.getX(), 1);
    EXPECT_EQ(b.getY(), 0);

    EXPECT_EQ(c.getX(), 1.1);
    EXPECT_EQ(c.getY(), -1);
}

TEST(Point, Setting) { //NOLINT
    Point c(1.1, -1);

    c.setX(2);
    c.setY(4);

    EXPECT_EQ(c.getX(), 2);
    EXPECT_EQ(c.getY(), 4);
}

TEST(Polygon, Init) { //NOLINT
    // If passed, then closed polyline and polyline are passed too.
    Point a(0, 0);
    Point b(1, 0);
    Point c(1, 0);
    Point d(0, -1);

    std::vector<Point> array = {a, b, c};
    EXPECT_ANY_THROW(Polygon e(array)); // 2 same points
    array[2].setX(2);
    EXPECT_ANY_THROW(Polygon e(array)); // points at one line

    array[0].setXY(0, 0);
    array[1].setXY(1, 0);
    array[2].setXY(2, 2);
    array.push_back(d);
    EXPECT_ANY_THROW(Polygon e(array)); // not convex

    Point x(0, 0);
    Point y (0, 1);
    Point z(0, 2);
    Point r(0, 3);
    std::vector<Point> arr = {x, y, z, r};
    EXPECT_ANY_THROW(ClosedPolyline v(arr)); // at same line
}

TEST(Polygon, SetAndPush) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(1, 1);
    Point d(0, 1);
    Point f(1, -1);

    std::vector<Point> array = {a, b, c};
    Polygon e(array);
    EXPECT_NO_THROW(e.push_point(d));
    EXPECT_ANY_THROW(e.push_point(f)); // Intersects itself
    EXPECT_ANY_THROW(e.change_point(a, 1, 0)); // Point already exists
    EXPECT_ANY_THROW(e.change_point(f, 100, 100)); // Point not found
    EXPECT_ANY_THROW(e.change_point(a, 100, -100)); // intersects itself
    EXPECT_NO_THROW(e.change_point(a, -1, 0));
}

TEST(Polygon, SquareAndPerimeter) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(1.1, -1);

    std::vector<Point> array = {a, b, c};
    Polygon d(array);

    EXPECT_EQ(d.get_square(), 0.5);

    d.change_point(c, 123, 456);
    c.setX(123);
    c.setY(456);

    EXPECT_EQ(d.get_square(), 228);
}

TEST(CorrectPolygon, Init) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(1.1, -1);

    std::vector<Point> array = {a, b, c};
}

TEST(Trapezium, InitAndChanging) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(1, 2);
    Point d(0, -1);
    Point e(-1, 0);

    std::vector<Point> array = {a, b, c};
    EXPECT_ANY_THROW(Trapezium f(array)); // Too few points
    array.push_back(d);
    array.push_back(e);
    EXPECT_ANY_THROW(Trapezium f(array)); // Too many points
    array.erase(array.end());
    EXPECT_ANY_THROW(Trapezium f(array)); // Intersects itself, not a trapezium
    array[3].setXY(-2, 1);
    EXPECT_ANY_THROW(Trapezium f(array)); // Not a trapezium
    array[2].setXY(1, 1);
    EXPECT_NO_THROW(Trapezium f(array));
    Trapezium f(array);
    EXPECT_ANY_THROW(f.change_point(array[2], 1, 2)); // Not a trapezium
    EXPECT_ANY_THROW(f.change_point(array[2], 0, 0)); // Already exists
}

TEST(Triangle, InitAndChanging) { //NOLINT
    Point a(0, 0);
    Point b(1, 0);
    Point c(2, 0);
    Point d(0, -1);

    std::vector<Point> array = {a, b};
    EXPECT_ANY_THROW(Triangle f(array)); // Too few points
    array.push_back(c);
    array.push_back(d);
    EXPECT_ANY_THROW(Triangle f(array)); // Too many points
    array.erase(array.end());
    EXPECT_ANY_THROW(Triangle f(array)); // Not a triangle
    array[2].setXY(2, 3);
    EXPECT_NO_THROW(Triangle f(array));
    Triangle f(array);
    EXPECT_ANY_THROW(f.change_point(array[2], 2, 0)); // Not a triangle
    EXPECT_ANY_THROW(f.change_point(array[2], 0, 0)); // Already exists
    EXPECT_NO_THROW(f.change_point(array[2],2, 4));
}

TEST(Additional, DynamicPolymorphism) { //NOLINT
    Point a(0, 0);
    Point b(2, 0);
    Point c(2, 2);
    Point d(0, 2);
    std::vector<Point> triangle_vector = {a, b, c};
    std::vector<Point> trapezium_vector = {a, b, c, d};


    EXPECT_NO_THROW(auto *e = new Point[4]);
    EXPECT_NO_THROW(auto *f = new Polyline[4]);
    auto *test_polymorph = new Polyline[10];
    EXPECT_NO_THROW(test_polymorph[0] = ClosedPolyline(triangle_vector));
    EXPECT_NO_THROW(test_polymorph[1] = Polygon(triangle_vector));
    EXPECT_NO_THROW(test_polymorph[2] = Triangle(triangle_vector));
    EXPECT_NO_THROW(test_polymorph[3] = Trapezium(trapezium_vector));
    EXPECT_NO_THROW(test_polymorph[4] = CorrectPolygon(trapezium_vector));

    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(test_polymorph[i].get_polyline());
    }

    delete[] test_polymorph;
    
    auto *test_figure_polymorph = new Polygon[10];
    EXPECT_NO_THROW(test_figure_polymorph[1] = Polygon(triangle_vector));
    EXPECT_NO_THROW(test_figure_polymorph[2] = Triangle(triangle_vector));
    EXPECT_NO_THROW(test_figure_polymorph[3] = Trapezium(trapezium_vector));
    EXPECT_NO_THROW(test_figure_polymorph[4] = CorrectPolygon(trapezium_vector));

    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(test_figure_polymorph[i].get_square());
        EXPECT_NO_THROW(test_figure_polymorph[i].get_perimeter());
        EXPECT_NO_THROW(test_figure_polymorph[i].get_polyline());
    }
}

TEST(Variable, Init) { //NOLINT
    EXPECT_NO_THROW(Variable a);
    EXPECT_NO_THROW(Variable a(1, "x", 2));
    EXPECT_NO_THROW(Variable a("+1x^2"));
    Variable a(1, "x", 2);
    EXPECT_DOUBLE_EQ(a.get_coefficient(), 1e0);
    EXPECT_DOUBLE_EQ(a.get_power(), 2e0);
    EXPECT_STREQ(a.get_name().c_str(), "x");
}

TEST(Polynom, Init) { //NOLINT
    Variable a;
    Variable b(1, "x", 2);
    Variable c(-2, "x", 1);
    EXPECT_NO_THROW(Polynom e(a));
    std::vector<Variable> d = {a, b, c};
    EXPECT_NO_THROW(Polynom e(d));
}

TEST(Polynom, Maths) { //NOLINT
    Variable a;
    Variable b(1, "x", 2);
    Variable c(-2, "x", 1);
    std::vector<Variable> d = {a, b, c};
    Polynom e(d);
    std::vector<std::pair< std::string, double>> f;
    f.emplace_back("x", 1e0);
    EXPECT_DOUBLE_EQ(e.get_value(f), 1e0 * pow(1e0, 2e0) - 2e0 * 1e0);
    f[0].second = 2e0;
    EXPECT_DOUBLE_EQ(e.get_value(f), 1e0 * pow(2e0, 2e0) - 2e0 * 2e0);
}

TEST(Polynom, Operators) { //NOLINT
    Variable a;
    Variable b(1, "x", 2);
    Variable c(-2, "x", 1);
    std::vector<Variable> d = {a, b, c};
    Polynom e(d);
    Polynom f(d);
    d.erase(d.begin());
    Polynom g(d);
    EXPECT_EQ(e == f, true);
    EXPECT_EQ(e == g, false);
}

TEST(Variable, Addition) { //NOLINT
    Variable a(1, "x", 1);
    Variable b(1, "x", 1);
    EXPECT_EQ((a + b).get_coefficient(), 2e0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
