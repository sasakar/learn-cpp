#include <iostream>

template <typename T>
class Pair1 {
private:
    T mX {};
    T mY {};

public:
    Pair1(const T& x, const T& y)
        : mX { x }, mY { y }
    {}

    T& first() { return mX; }
    T& second() { return mY; }
    const T& first() const { return mX; }
    const T& second() const { return mY; }
};

int main() {
    Pair1<int> p1 { 5, 8 };
    std::cout << "Pair: " << p1.first() << " " << p1.second() << "\n";

    const Pair1<double> p2 { 2.3, 4.5 };
    std::cout << "Pair: " << p2.first() << " " << p2.second() << "\n";

    return 0;
}
