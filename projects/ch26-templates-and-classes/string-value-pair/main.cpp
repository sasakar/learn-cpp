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

template <typename S>
class StringValuePair : public Pair<std::string, S> {
public:
    StringValuePair(std::string_view key, const S& value)
        // a std::string_view won't implicitly convert to a std::string, we must
        // be explicit
        : Pair<std::string, S>{ static_cast<std::string>(key), value }
    {}
};

int main() {
	StringValuePair<int> svp{ "Hello", 5 };
	std::cout << "Pair: " << svp.first() << ' ' << svp.second() << '\n';

	return 0;
}
