#include <iostream>

class Point {
private:
    int mX {};
    int mY {};

public:
    Point(int x, int y)
        : mX { x }, mY { y }
    {}

    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        return out << "Point(" << p.mX << ", " << p.mY << ")";
    }
};
class Shape {
public:
    virtual std::ostream& print(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Shape& p) {
        return p.print(out);
    }

    virtual ~Shape() = default;
};

class Triangle : public Shape {
private:
    Point mP1;
    Point mP2;
    Point mP3;

public:
    Triangle(const Point& p1, const Point& p2, const Point& p3)
        : mP1 { p1 }, mP2 { p2 }, mP3 { p3 }
    {}

    std::ostream& print(std::ostream& out) const override {
        return out << "Triangle(" << mP1 << ", " << mP2 << ", " << mP3 << ")";
    }
};

class Circle : public Shape {
private:
    Point mCenter;
    int mRadius;

public:
    Circle(const Point& center, int radius)
        : mCenter { center }, mRadius { radius }
    {}

    std::ostream& print(std::ostream& out) const override {
        return out << "Circle(" << mCenter << ", radius " << mRadius << ")";
    }
};

int main() {
    Circle c { Point { 1, 2 }, 7 };
    std::cout << c << "\n";

    Triangle t { Point { 1, 2 }, Point { 3, 4 }, Point { 5, 6 } };
    std::cout << t << "\n";

    return 0;
}
