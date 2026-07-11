#include <iostream>
#include <string>
using namespace std;

// blueprint of a car's model (Abstract Class)
class Car {
    public: 
        // virtual keyword is used when we want to define the working of a function in it's child class
        virtual void startEngine()= 0;
        virtual void shiftGear(int gear)= 0;
        virtual void accelarate()= 0;
        virtual void brake()= 0;
        virtual void stopEngine()= 0;
        virtual ~Car() {}
};

class SportsCar : public Car {
    public:
        string brand;
        string model;
        bool isEngineOn;
        int currentSpeed;
        int currentGear;

        // constructor
        SportsCar(string b, string m){
            this->brand = b;
            this->model = m;
            isEngineOn = false;
            currentSpeed = 0;
            currentGear = 0;
        }

        // methods
        void startEngine(){
            isEngineOn = true;
            cout << brand << " " << model << " : Engine starts!" << endl;
        }

        void shiftGear(int gear) {
            if(!isEngineOn){
                cout << brand << " " << model << " : Engine is off. Turn it ON to shift the gear" << endl;
                return;
            }

            currentGear = gear;
            cout << brand << " " << model << " : Shift to grear " << currentGear << endl;
        }

        void accelarate(){
            if(!isEngineOn){
                cout << brand << " " << model << " : Engine is off. Turn it ON to accelarate" << endl;
                return;
            }

            currentSpeed += 20;
            cout << brand << " " << model << " : Accelarating to " << currentSpeed << " km/h" << endl;
        }

        void brake() {
            currentSpeed -= 20;
            if(currentSpeed < 0) currentSpeed = 0;
            cout << brand << " " << model << " : Braking! Current speed is: " << currentSpeed << " km/h" << endl;
        }

        void stopEngine() {
            isEngineOn = false;
            currentGear = 0;
            currentSpeed = 0;
            cout << brand << " " << model << " : Engine turned OFF. " << endl;
        }
};

int main() {
    Car* newCar = new SportsCar("Ferrari", "Enzo");

    newCar->startEngine();
    newCar->shiftGear(1);
    newCar->accelarate();
    newCar->shiftGear(2);
    newCar->accelarate();
    newCar->accelarate();
    newCar->shiftGear(3);
    newCar->accelarate();
    newCar->accelarate();
    newCar->brake();
    newCar->brake();
    newCar->brake();
    newCar->stopEngine();
    
    return 0;
}
