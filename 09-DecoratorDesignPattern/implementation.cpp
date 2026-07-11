#include <iostream>
#include <string>
using namespace std;

class Character {
public:
    virtual string getAbilities() const = 0;
    virtual ~Character() {}
};

// Concerte Component: Mario Character
class Mario : public Character {
public:
    string getAbilities() const override {
        return "Marii";
    }
};

// Abstract Decorator : CharacterDecorator
class CharacterDecorator : public Character {
protected:
    Character* character;
public:
    CharacterDecorator(Character* c){
        this->character = c;
    }
};


// Concrete decorator : heightUp Ability
class HeightUpDecorator : public CharacterDecorator {
public:
    HeightUpDecorator(Character* c) : CharacterDecorator(c) {}

    string getAbilities() const override {
        return character->getAbilities() + " with HeightUP";
    }
};


// concrte decorator : gun power ability
class GunPowerDecorator : public CharacterDecorator {
public:
    GunPowerDecorator(Character* c) : CharacterDecorator(c) {}

    string getAbilities() const override {
        return character->getAbilities() + " with Gun";
    }
};


// concrete decorator : star power ability
class StarPowerDecorator : public CharacterDecorator {
public:
    StarPowerDecorator(Character* c) : CharacterDecorator(c) {}

    string getAbilities() const override {
        return character->getAbilities() + " with Star";
    }
};


int main() {
    // basic mario character
    Character* mario = new Mario();
    cout << "Basic Character: " << mario->getAbilities() << endl;

    // now integrate height up ability
    mario = new HeightUpDecorator(mario);
    cout << "After Heightup: " << mario->getAbilities() << endl;

    // now integrate gun power ability
    mario = new GunPowerDecorator(mario);
    cout << "After Gun Power: " << mario->getAbilities() << endl;

    // now integrate star power ability
    mario = new StarPowerDecorator(mario);
    cout << "After star power: " << mario->getAbilities() << endl;

    return 0;
}