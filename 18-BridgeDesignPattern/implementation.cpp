/* Bridge Design Pattern                                                                                                                                                        Date: 02/07/2026 */
#include <iostream>
#include <string>
using namespace std;

// Implementation : LLP (Low Level Part)
class Engine {
public: 
    virtual void start() = 0;
    virtual ~Engine() {}
};

// concrete implementors
class PetrolEngine : public Engine {
public: 
    void start() override {
        cout << "Petrol engine starting!" << endl;
    }
};

class DieselEngine : public Engine {
public: 
    void start() override {
        cout << "Diesel engine starting!" << endl;
    }
};

class ElectricEngine : public Engine {
public: 
    void start() override {
        cout << "Electric engine starting!" << endl;
    }
};


// abstraction : HLP (High Level Part)
class Car {
protected:
    Engine* engine;
public:
    Car(Engine* e) {
        this->engine = e;
    }
    virtual void drive() = 0;
};


// concrete abstractions
class Sedan : public Car {
public: 
    Sedan(Engine* e) : Car(e) {}

    void drive() override {
        engine->start();
        cout << "Driving a Sedan!" << endl;
    }
};

class SUV : public Car {
public: 
    SUV(Engine* e) : Car(e) {}

    void drive() override {
        engine->start();
        cout << "Driving a SUV!" << endl;
    }
};


int main() {
    Engine* pEngine = new PetrolEngine();
    Engine* dEngine = new DieselEngine();
    Engine*  eEngine = new ElectricEngine();

    // create car
    Car* sedan = new Sedan(pEngine);
    Car* suv = new SUV(dEngine);
    Car* electricSedan = new Sedan(eEngine);

    sedan->drive();
    suv->drive();
    electricSedan->drive();

    delete sedan;
    delete suv;
    delete electricSedan;
    
    return 0;
}
