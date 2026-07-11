#ifndef CART_H
#define CART_H

#include <iostream>
#include <vector>
#include <string>
#include "../models/MenuItem.h"
#include "../models/Restaurant.h"
using namespace std;

class Cart {
private:
    Restaurant* restaurant;
    vector<MenuItem> items;

public:
    // constructor
    Cart() {
        restaurant = nullptr;
    }

    // getters & setters
    void addItem(const MenuItem& item) {
        if(!restaurant) {
            cerr << "Cart: set a retaurant before adding items" << endl;
            return;
        }
        items.push_back(item);
    }

    double getTotalCost() const {
        double sum = 0;
        for(const auto& ite : items) {
            sum += ite.getPrice();
        }

        return sum;
    }

    bool isEmpty() {
        return (!restaurant || items.empty());
    }

    void clear() {
        items.clear();
        restaurant = nullptr;
    }

    // getter & setters
    void setRestaurant(Restaurant* r){
        restaurant = r;
    }

    Restaurant* getRestaurant() const {
        return restaurant;
    }

    const vector<MenuItem> &getItems() const {
        return items;
    }
};

#endif