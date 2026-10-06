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

    if (monster.isDead()) {
        std::cout << "You killed the " << monster.getName() << "\n";
        player.levelUp();
        std::cout << "You are now level " 
                    << player.getLevel() << "\n";
        std::cout << "You found " << monster.getGold() << " gold.\n";
        player.addGold(monster.getGold());
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
                  << player.getGold() << ". Congratulations :)\n";
    }

	return 0;
}
