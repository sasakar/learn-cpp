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
protected: // Protected so only derived classes can access
    Apple(std::string_view name, std::string_view color)
        : Fruit { name, color }
    {}
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

class GrannySmith : public Apple {
public:
    GrannySmith()
        : Apple { "granny smith apple", "green" }
    {}
};


int main()
{
	Apple a{ "red" };
	Banana b{};
    GrannySmith c{};

	std::cout << "My " << a.get_name() << " is " << a.get_color() << ".\n";
	std::cout << "My " << b.get_name() << " is " << b.get_color() << ".\n";
    std::cout << "My " << c.get_name() << " is " << c.get_color() << ".\n";

	return 0;
}
