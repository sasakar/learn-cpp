#include "Random.h" // defines Random::mt, Random::get(), and Random::generate()

#include <iostream>
#include <string>
#include <string_view>
#include <cctype>

class Creature {
protected:
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
    void reduceHealth(int health) { mHealth -= health; }
    bool isDead() { return mHealth <= 0; }
    void addGold(int amount) { mGold += amount; }
    void addDamage(int damage) { mDamage += damage; }
};

class Player : public Creature {
private:
    int mLevel { 1 };

public:
    Player(std::string_view name)
        : Creature { name, '@', 10, 1, 0}
    {}

    void levelUp() {
        ++mLevel;
        ++mDamage;
    }

    int getLevel() const { return mLevel; }
    bool hasWon() { return mLevel >= 20; }
};

class Monster : public Creature {
public:
    enum Type {
        dragon,
        orc,
        slime,
        max_types
    };

    static inline Creature monsterData[] {
        {"dragon", 'D', 20, 4, 100},
        {"orc", 'o', 4, 2, 25},
        {"slime", 's', 1, 1, 10}
    };

    Monster(Type type)
        : Creature { monsterData[type] }
    {}

    static const Monster getRandomMonster() {
        int num { Random::get(0, Type::max_types-1) };
        return Monster { static_cast<Type>(num) };
    }
};

char getChoice() {
    char choice {};
    while (true) {
        std::cout << "(R)un or (F)ight: ";
        std::cin >> choice;
        choice = static_cast<char>(
            std::towlower(static_cast<unsigned char>(choice)));

        if (choice == 'r' || choice == 'f') {
            return choice;
        }

        std::cout << "Invalid choice, try again.\n";
    }
}

void attackPlayer(Monster& m, Player& player) {
    std::cout << "The " << m.getName() << " hit you for " 
              << m.getDamage() << " damage.\n";
    player.reduceHealth(m.getDamage());
}

void attackMonster(Monster& m, Player& player) {
    std::cout << "You hit the " << m.getName() << " for "
              << player.getDamage() << " damage.\n";

    m.reduceHealth(player.getDamage());

    if (m.isDead()) {
        std::cout << "You killed the " << m.getName() << "\n";
        player.levelUp();
        std::cout << "You are now level " 
                    << player.getLevel() << "\n";
        std::cout << "You found " << m.getGold() << " gold.\n";
        player.addGold(m.getGold());
    } else {
        attackPlayer(m, player);
    }
}

void fightMonster(Player& player) {
    char choice {};
    while (true) {
        Monster m { Monster::getRandomMonster() };
        std::cout << "You have encountered a " << m.getName() 
                  << " (" << m.getSymbol() << ").\n";
    
        while (true) {
            if (player.isDead() || m.isDead()) {
                break;
            }

            choice = getChoice();
            if (choice == 'r') {
                bool hasEscaped { static_cast<bool>(Random::get(0, 1)) };
                if (hasEscaped) {
                    std::cout << "You successfully fled.\n";
                    break;
                } else {
                    std::cout << "You failed to flee.\n";
                    attackPlayer(m, player);
                    continue;
                }
            } 
            else if (choice == 'f') {
                attackMonster(m, player);
            }
        } 
        
        if (player.isDead() || player.hasWon()) {
            break;
        }
    }
}

int main()
{
    std::cout << "Enter your name: ";
    std::string name {};
    std::cin >> name;
    Player player { name };
    std::cout << "Welcome, " << player.getName() << "\n";
    std::cout << "You have " << player.getHealth() << " health and are "
              << "carrying " << player.getGold() << " gold.\n";

    fightMonster(player);

    if(player.hasWon()) {
        std::cout << "You won at level " << player.getLevel() << " with "
                  << player.getGold() << ". Congratulations :)\n";
    }

    if (player.isDead()) {
        std::cout << "You died at level " << player.getLevel() << " with "
                  << player.getGold() << " gold.\n";
        std::cout << "Too bad you can't take it with you :(\n";
    }

	return 0;
}
