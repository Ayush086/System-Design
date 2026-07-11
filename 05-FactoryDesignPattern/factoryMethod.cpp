#include <iostream>
using namespace std;

class Burger {
public:
    virtual void prepare() = 0;
    virtual ~Burger() {}
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

class BasicWheatBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing basic wheat burger only with patty" << endl;
    }
};

class StandardWheatBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing standard wheat burger with patty and bun" << endl;
    }
};

class PremiumWheatBurger : public Burger {
public:
    void prepare() override {
        cout << "preparing premium wheat burger with premium patty, cheese and bun" << endl;
    }
};


class BurgerFactory {
public:
    virtual Burger* createBurger(string& type) = 0;
};


class SinghBurger : public BurgerFactory {
public:
    Burger* createBurger(string& type) override {
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

class KingBurger : public BurgerFactory {
public:
    Burger* createBurger(string& type) override {
        if (type == "basic") {
            return new BasicWheatBurger();
        } else if (type == "standard") {
            return new StandardWheatBurger();
        } else if (type == "premium") {
            return new PremiumWheatBurger();
        } else {
            cout << "invalid buger type!" << endl;
            return nullptr;
        }
    }
};


int main() {
    string type = "basic";

    BurgerFactory* myFactory = new KingBurger();
    Burger* burger = myFactory->createBurger(type);

    burger->prepare();

    cout << "-------------------" << endl;
    string type2 = "standard";
    BurgerFactory* myFactory2 = new SinghBurger();
    Burger* burger2 = myFactory2->createBurger(type2);
    burger2->prepare();

    return 0;
}
