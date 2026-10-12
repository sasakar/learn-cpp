#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
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

    int getRadius() const { return mRadius; }
};

int getLargestRadius(const std::vector<std::unique_ptr<Shape>>& v) {
	int largestRadius{ 0 };

	// Loop through all the shapes in the vector
	for (const auto& element : v) {
		// // Ensure the dynamic cast succeeds by checking for a null pointer result
		if (auto *c { dynamic_cast<const Circle*>(element.get()) })
		{
			largestRadius = std::max(largestRadius, c->getRadius());
		}
	}

	return largestRadius;
}

int main() {
	std::vector<std::unique_ptr<Shape>> v;
	v.reserve(3);
	v.push_back(std::make_unique<Circle>(Point{1, 2}, 7));
	v.push_back(std::make_unique<Triangle>(Point{1, 2}, Point{3, 4}, Point{5, 6}));
	v.push_back(std::make_unique<Circle>(Point{7, 8}, 3));

	for (const auto& element : v) std::cout << *element << '\n';

	std::cout << "The largest radius is: " << getLargestRadius(v) << '\n';
}
