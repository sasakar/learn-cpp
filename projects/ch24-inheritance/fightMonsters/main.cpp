#include "Random.h" // defines Random::mt, Random::get(), and Random::generate()

#include <iostream>
#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <cctype>

class Potion {
public:
    // All possible types of potions
    enum Type {
        health,
        strength,
        poison,

        // For random generation
        max_type
    };

    enum Size {
        small,
        medium,
        large,

        max_size
    };

private:
    Type mType {};
    Size mSize {};

public:
    Potion(Type type, Size size)
        : mType { type }, mSize { size }
    {}

    Type getType() const { return mType; }
    Size getSize() const { return mSize; }

    // The names of potions are compile-time literals, we can
    // return a std::string_view
    static std::string_view getPotionTypeName(Type type) {
        static constexpr std::string_view names[] {
            "Health",
            "Strength",
            "Poison"
        };

        return names[type];
    }

    static std::string_view getPotionSizeName(Size size) {
        static constexpr std::string_view names[] {
            "Small",
            "Medium",
            "Large"
        };

        return names[size];
    }

    std::string getName() const {
        std::stringstream result {};
        result << getPotionSizeName(getSize()) << " potion of " 
               << getPotionTypeName(getType());

        // We can extract the string from an std::stringstream using str()
        // member function
        return result.str();

          
        // C++ 20's format is an even cleaner option
        // return std::format("{} potion of {}", getPotionSizeName(getSize()), 
        //                     getPotionTypeName(getType()));
    }

    static Potion getRandomPotion() {
        return Potion {
            static_cast<Type>(Random::get(0, max_type - 1)),
            static_cast<Size>(Random::get(0, max_size - 1))
        };
    }
};
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
    bool isDead() const { return mHealth <= 0; }
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

    // Applies a potion's effect to the player
    void drinkPotion(const Potion& potion) {
        switch(potion.getType()) {
            case Potion::health:
                // Only a health potion's size affects its power. All other
                // potions are independent of size
                mHealth += ((potion.getSize() == Potion::large) ? 5 : 2);
                break;
            case Potion::strength:
                ++mDamage;
                break;
            case Potion::poison:
                reduceHealth(1);
                break;
                // Handle max_type to silence the compiler warning, don't use
                // default: because we want the compiler to warn us if add a new
                // potion but forget to implement its effect.
            case Potion::max_type:
                break;
        }
    }
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

void onMonsterKilled(Player& player, const Monster& monster) {
    std::cout << "You killed the " << monster.getName() << ".\n";
    player.levelUp();
    std::cout << "You are now level " << player.getLevel() << ".\n";
    std::cout << "You found " << monster.getGold() << " gold.\n";
    player.addGold(monster.getGold());

    // 30% chance of finding a potion
    constexpr int potionChance { 30 };
    if (Random::get(1, 100) <= potionChance) {
        // Generate a random potion
        auto potion { Potion::getRandomPotion() };

        std::cout << "You found a mythical potion! Do you want to drink it? "
                  << "[y/n]: ";
        char choice {};
        std::cin >> choice;

        if (choice == 'Y' || choice == 'y') {
            // Apply the effect
            player.drinkPotion(potion);
            // Reveal the potion type and size
            std::cout << "You drank a " << potion.getName() << ".\n";
        }
    }

}

void attackPlayer(const Monster& monster, Player& player) {
    // If the monster is dead, it can't attack the player
    if (monster.isDead()) {
        return;
    }

    std::cout << "The " << monster.getName() << " hit you for " 
              << monster.getDamage() << " damage.\n";
    player.reduceHealth(monster.getDamage());
}

void attackMonster(Player& player, Monster& monster) {
    // If the player is dead, we can't attack the monster
    if (player.isDead()) {
        return;
    }
    std::cout << "You hit the " << monster.getName() << " for "
              << player.getDamage() << " damage.\n";

    monster.reduceHealth(player.getDamage());

    // If the monster is now dead, level the player up
    if (monster.isDead()) {
        // Reward the player
        onMonsterKilled(player, monster);
    }
}

void fightMonster(Player& player) {

    Monster monster { Monster::getRandomMonster() };
    std::cout << "You have encountered a " << monster.getName() 
                << " (" << monster.getSymbol() << ").\n";

    char choice {};
    
    // While the monster isn't dead and the player isn't dead, the fight goes on
    while (!monster.isDead() && !player.isDead()) {
        choice = getChoice();
        if (choice == 'r') {
            // 50% chance of fleeing successfully
            bool hasEscaped { static_cast<bool>(Random::get(0, 1)) };
            if (hasEscaped) {
                std::cout << "You successfully fled.\n";
                return; // success ends the encounter
            } else {
                // Failure to flee gives the monster a free attack on the player
                std::cout << "You failed to flee.\n";
                attackPlayer(monster, player);
                continue;
            }
        } 
        else if (choice == 'f') {
            // Player attacks first, monster attacks second
            attackMonster(player, monster);
            attackPlayer(monster, player);
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

    // If the player isn't dead and hasn't won yet, the game continues
    while (!player.isDead() && !player.hasWon()) {
        fightMonster(player);
    }

    // At this point, the player is either dead or has won
    if (player.isDead()) {
        std::cout << "You died at level " << player.getLevel() << " with "
                  << player.getGold() << " gold.\n";
        std::cout << "Too bad you can't take it with you :(\n";
    } else {
        std::cout << "You won at level " << player.getLevel() << " with "
                  << player.getGold() << " gold. Congratulations :)\n";
    }

	return 0;
}
