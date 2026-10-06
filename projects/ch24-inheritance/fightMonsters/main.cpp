#include <iostream>
#include <string>
#include <string_view>

class Creature {
private:
    std::string mName {};
    char mSymbol {};
    int mHealth {};
    int mDamage {};
    int mGold {};

public:
    Creature(std::string_view name, char symbol, int health, int damage, 
        int gold)
        : mName { name }, mSymbol { symbol }, mHealth { health }, 
          mDamage { damage }, mGold { gold }
    {}

    const std::string& getName() const { return mName; }
    char getSymbol() const { return mSymbol; }
    int getHealth() const { return mHealth; }
    int getDamage() const { return mDamage; }
    int getGold() const { return mGold; }

    void reduceHealth(int damage) {
        mHealth -= damage;
    }

    bool isDead() {
        if (mHealth < 0) {
            return true;
        }

        return false;
    }

    void addGold(int amount) {
        mGold += amount;
    }

    void addDamage(int damage) {
        mDamage += damage;
    }

};

class Player : public Creature {
private:
    int mLevel { 1 };

public:
    Player(std::string_view name)
        : Creature { name, '@', 10, 1, 0}
    {}

    void levelUp() {
        mLevel += 1;
        addDamage(1);
    }

    int getLevel() const { return mLevel; }

    bool hasWon() {
        if (mLevel == 20) {
            return true;
        }

        return false;
    }
};

int main()
{
	Creature o{ "orc", 'o', 4, 2, 10 };
	o.addGold(5);
	o.reduceHealth(1);
	std::cout << "The " << o.getName() << " has " << o.getHealth() << " health and is carrying " << o.getGold() << " gold.\n";

    std::cout << "Enter your name: ";
    std::string name {};
    std::cin >> name;
    Player player { name };
    std::cout << "Welcome, " << player.getName() << "\n";
    std::cout << "You have " << player.getHealth() << " health and are "
              << "carrying " << player.getGold() << " gold.\n";

	return 0;
}
