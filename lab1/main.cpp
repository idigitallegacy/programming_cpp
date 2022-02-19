//
// Created by Михаил on 18.02.2022.
//

#include "linker/linker.h"
#include <iostream>

int main() {
    Point a(0, 0);
    Point b(1, 0);
    Point c(1, 1);
    Point d(0, 1);
    Point f(1, -1);

    std::vector<Point> array = {a, b, c};
    try {
        Polygon e(array);
        e.push_point(d);
        e.push_point(f); // Intersects itself
        e.change_point(a, 1, 0); // Point already exists
        e.change_point(f, 100, 100); // Point not found
        e.change_point(a, 100, -100); // intersects itself
        e.change_point(a, -1, 0);
    } catch (FigureException &exception) {
        std::cout << exception.what() <<'\n';
    }


    return 0;
}