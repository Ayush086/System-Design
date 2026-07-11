#include <iostream>
using namespace std;
// proudct-1
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

// product-2
class GarlicBread{
public:
    virtual void prepare() = 0;
};

class BasicGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing basic garlic bread only with patty" << endl;
    }
};

class StandardGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing standard garlic bread with patty and bun" << endl;
    }
};

class PremiumGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing premium garlic bread with premium patty, cheese and bun" << endl;
    }
};

class BasicWheatGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing basic wheat garlic bread only with patty" << endl;
    }
};

class StandardWheatGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing standard wheat garlic bread with patty and bun" << endl;
    }
};

class PremiumWheatGB : public GarlicBread {
public:
    void prepare() override {
        cout << "preparing premium wheat garlic bread with premium patty, cheese and bun" << endl;
    }
};


class MealFactory{
public:
    virtual Burger* createBurger(string& type) = 0;
    virtual GarlicBread* createGB(string& type) = 0;
};


class SinghBurger : public MealFactory {
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

    GarlicBread* createGB(string& type) override {
        if (type == "basic") {
            return new BasicGB();
        } else if (type == "standard") {
            return new StandardGB();
        } else if (type == "premium") {
            return new PremiumGB();
        } else {
            cout << "invalid garlic bread type!" << endl;
            return nullptr;
        }
    }
};


class KingBurger : public MealFactory {
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

    GarlicBread* createGB(string& type) override {
        if (type == "basic") {
            return new BasicWheatGB();
        } else if (type == "standard") {
            return new StandardWheatGB();
        } else if (type == "premium") {
            return new PremiumWheatGB();
        } else {
            cout << "invalid garlic bread type!" << endl;
            return nullptr;
        }
    }
};


int main() {
    string burgerType = "basic";
    string gbType = "standard";

    MealFactory* myFactory = new KingBurger();
    Burger* burger = myFactory->createBurger(burgerType);
    GarlicBread* gb = myFactory->createGB(gbType);

    burger->prepare();
    gb->prepare();

    cout << "-------------------" << endl;
    
    MealFactory* myFactory2 = new SinghBurger();
    Burger* burger2 = myFactory2->createBurger(burgerType);
    GarlicBread* gb2 = myFactory2->createGB(gbType);
    burger2->prepare();
    gb2->prepare();

    return 0;
}