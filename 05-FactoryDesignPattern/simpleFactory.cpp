#include <iostream>
using namespace std;

class Burger {
public:
    virtual void prepare () = 0;
    virtual ~Burger() {} // destructor
};


class BasicBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing basic burger only with patty" << endl;
    }
};

class StandardBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing standard burger with patty and bun" << endl;
    }
};

class PremiumBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing premium burger with premium patty, cheese and bun" << endl;
    }
};


class BurgerFactory {
public:
    Burger* createBurger(string& type) {
        if (type == "basic") {
            return new BasicBurger();
        } else if (type == "standard") {
            return new StandardBurger();
        } else if (type == "premium") {
            return new PremiumBurger();
        } else {
            cout << "invalid buger type!" << endl;
            return nullptr;
        }
    }
};



int main() {
    string type = "basic";

    BurgerFactory* myBurgerFactory = new BurgerFactory();

    Burger* burger = myBurgerFactory->createBurger(type);
    burger->prepare();

    return 0;
}
