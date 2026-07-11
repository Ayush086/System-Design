#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
using namespace std;

class MenuItem {
private:
    string code;
    string name;
    int price;

public:
    // constructor
    MenuItem(const string& code, const string& name, int price) {
        this->code = code;
        this->name = name;
        this->price = price;
    }

    // getters & setters
    string getCode() const {
        return code;
    }

    void setCode(string &c) {
        code = c;
    }

    string getName() const {
        return name;
    }

    void setName(string &n) {
        name = n;
    }

    int getPrice() const {
        return price;
    }

    void setPrice(int &p) {
        price = p;
    }
};

#endif