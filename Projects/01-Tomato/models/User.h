#ifndef USER_H
#define USER_H

#include <string>
#include "Cart.h"
using namespace std;

class User {
private:
    int userId;
    string name;
    string address;
    Cart* cart;

public:
    // constructor
    User(int userId, const string& name, const string& address) {
        this->userId = userId;
        this->name = name;
        this->address = address;
        cart = new Cart();
    }

    // destructor
    ~User() {
        delete cart;
    }

    // getters & setters
    // int getUserId() const {
    //     return userId;
    // }

    // void setUserId(int &id) {
    //     userId = id;
    // }

    string getName() const {
        return name;
    }

    void setName(string &n) {
        name = n;
    }

    string getAddress() const {
        return address;
    }

    void setAddress(string &addr) {
        address = addr;
    }

    Cart* getCart() const {
        return cart;
    }
};

#endif