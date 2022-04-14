# Lab 1. Classes. Inheritance.

## Polygons
### Target
To design and realize the next classes:
- Point
- Polyline
- Closed polyline
- Polygon
- Triangle
- Trapezium
- Correct polygon

For each of them, to realize:
- Constructor(s)
- Copy constructor
- Equality operator (=)
- Perimeter computation (if possible)
- Square computation (if possible)

### Code description
The following inheritance is realized:
- Point
- Polyline
	- Closed Polyline
		- Polygon
			- Triangle
			- Trapezium
			- Correct Polygon

Classes above are described at relevant files at **`/class/figure/`**. Custom exceptions are realized via **`#define`** macro, and described at **`/class/custom_exceptions.h`**.

### Code usage
First of all, you need to include all of the files.
*Note: this realization also requires STL: `<vector>, <string>, <cmath>, <algorithm>`*

To avoid multiple **`#include`** macros at main, you could include **`/linker/linker.h`** I designed (*Note: this file also requires Polynom and Variable classes. Read about them below.* ):
```
    #include "linker/linker.h"
```
After that, inside `int main() {}`, initialize point:
```
Point start_points(0e0, 0e0); // Initializes point at (0; 0);
```
Each other structure initializes with a vector of Points. You can use the following vectors for testing purposes:
```
std::vector<Point> polyline= { Point(0, 0), Point(1,1), Point(1, -1), Point(-1, -1) };
std::vector<Point> closed_polyline = polyline;
std::vector<Point> polygon = { Point(0, 0), Point(0, 1), Point(1, 2), Point(2, 3), Point(3, 0) };
std::vector<Point> triangle ={ Point(0, 0), Point(1, 0), Point(0, 1) };
std::vector<Point> trapezium = { Point(0, 0), Point(1, 1), Point(2, 1), Point(3, 0) };
std::vector<Point> correct_polygon = { Point(0, 0), Point(0, 1), Point(1, 1), Point(1, 0) };
```
Now you are ready to initialize your first class! Let's do it:
```
Polyline test_polyline(polyline);
```
And even use your first setter:
```
test_polyline.push_point(Point(10, 10));
```
Or getter:
```
std::cout << test_polyline.get_length() << '\n';
```
### Methods
✓ - available
\* - virtual
✕ - unavailable

| Method | Point | Polyline | Closed Polyline | Polygon | Triangle | Trapezium | Correct Polygon |
| ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ |
| setX | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ |
| setY | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ |
| setXY | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ |
| getX | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ |
| getY | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ |
| change_point | ✕ | ✓ | * | ✓ | ✓ | ✓ | ✓ |
| push_point | ✕ | ✓ | * | ✓ | ✓ | ✓ | * |
| erase | ✕ | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ |
| reshape | ✕ | ✕ | ✕ | ✕ | ✕ | ✕ | ✓ |
| get_length | ✕ | ✓ | * | * | * | * | * |
| get_polyline | ✕ | ✓ | ✓ | ✕ | ✕ | ✕ | ✕ |
| get_size | ✕ | ✓ | ✕ | ✕ | ✕ | ✕ | ✕ |
| get_square | ✕ | ✕ | ✕ | ✓ | * | * | * |
| get_perimeter | ✕ | ✕ | ✕ | ✓ | * | * | * |

Methods usage:
`setX(const double x)` sets X coordinate of point

`setY(const double y)` sets Y coordinate of point

`setXY(const double x, const double y)` sets (X; Y) coordinates of point

`double getX()` returns X coordinate of point

`double getY()` gets Y coordinate of point

`change_point(const Point &needle, const double new_x, const double new_y)` Finds a needle item at self and sets its coordinates to (new_x, new_y)

`push_point(const Point &needle)` Pushes point in self

`erase(const Point &needle)` Erases point needle in self

`reshape(const double multiplier)` Multiplies each point in self by multiplier

`double get_length()` Returns line length

`double get_perimeter()` get_length() decorator

`std::vector<Point> get_polyline()` Returns current state of points

`long long get_size()` Returns amount of stored points

`double get_sqruare()` Returns square of a shape

## Polynoms
### Target
To design and realize the class that describes simple polynom (infinite summ of a(i) \* x(i) \^ b(i)). At this class must be: 
- equality operator (=)
- boolean equality operator (==, !=)
- summ operators (+, -, ++, --, +=, -=)
- multiplicative operators (\*, / (by the number), \*=, /=)
- input operators (>>, <<)
- getter operator ([i] to get x(i))

### Realization limitations
For correct input, you must escape operators by `'\'`. E.g.:
```
>>> x^2  \-  x^3
>>> 23x^3 \+ 4y^2
```
Don't use multiple variable names (x, y, z at one polynom). It's allowed, but doesn't work fine for now.

### Code description
Polynom - is a vector of Variable classes. Variable stores in tself variable coefficient, name and power.
Each class is described at **`/classes/polynom/`** dir.

### Code usage
First of all, you need to include all of the files.
*Note: this realization also requires STL: `<vector>, <string>, <cmath>`*

To avoid multiple **`#include`** macros at main, you could include **`/linker/linker.h`** I designed (*Note: this file also requires Polygons classes. Read about them above.* ):
```
    #include "linker/linker.h"
```

Initialize your variable:
```
Variable x(1, 'x', 1); // generates 1x^1 variable
```
Generate a polynom:
```
Polynom polynom(x);
```
So, that's all :)
All of the operators are realized fine, so you can use them as usual.
Also, you can get the polynom value, assuming variables are some numbers via `get_value()` method. E.g.:
```
std::vector<std::pair<std::string, double>> variables_table = { std::make_pair("x", 2) };
std::cout << polynom.get_value(variables_table) << '\n';
// Output: 2.
```
