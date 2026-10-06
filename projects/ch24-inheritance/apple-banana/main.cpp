#include <iostream>
#include <string>
#include <string_view>

class Fruit {
private:
    std::string m_name {};
    std::string m_color {};

public:
    Fruit(std::string_view name = "", std::string_view color = "")
        : m_name { name }, m_color { color }
    {}

    const std::string& get_name () const { return m_name; }
    const std::string& get_color () const { return m_color; }
};

class Apple : public Fruit {
public:
    Apple(std::string_view color = "red")
        : Fruit { "apple", color}
    {}
};

class Banana : public Fruit {
public:
    Banana()
        : Fruit { "banana", "yellow" }
    {}
};


int main()
{
	Apple a{ "red" };
	Banana b{};

	std::cout << "My " << a.get_name() << " is " << a.get_color() << ".\n";
	std::cout << "My " << b.get_name() << " is " << b.get_color() << ".\n";

	return 0;
}
