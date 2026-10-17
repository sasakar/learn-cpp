#include <iostream>

template <typename T, typename S>
class Pair {
private:
    T mX {};
    S mY {};

public:
    Pair(const T& x, const S& y)
        : mX { x }, mY { y }
    {}

    T& first() { return mX; }
    S& second() { return mY; }
    const T& first() const { return mX; }
    const S& second() const { return mY; }
};

int main() {
	Pair<int, double> p1 { 5, 6.7 };
	std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

	const Pair<double, int> p2 { 2.3, 4 };
	std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';

	return 0;
}
