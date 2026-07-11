#include <iostream>
#include <string>
using namespace std;

class ManualCar {
    private:
        string brand;
        string model;
        bool isEngineOn;
        int currentSpeed;
        int currentGear;

    public:
        ManualCar(string b, string m) { 
            this->brand = b;
            this->model = m;
            this->isEngineOn = false;
            this->currentSpeed = 0;
            this->currentGear = 0;
        }

        void startEngine(){
            isEngineOn = true;
            cout << brand << " " << model << " : Engine starts!" << endl;
        }

        void stopEngine() {
            isEngineOn = false;
            currentGear = 0;
            currentSpeed = 0;
            cout << brand << " " << model << " : Engine turned OFF. " << endl;
        }

        // overloading accelerate() - Static Polymorphism
        void accelarate(){
            if(!isEngineOn){
                cout << brand << " " << model << " : Cannot accelerate! Engine is off" << endl;
                return;
            }
            currentSpeed += 20;
            cout << brand << " " << model << " : Accelerating to " << currentSpeed << " km/h" << endl;
        }
        
        void accelarate(int speed){
            if(!isEngineOn){
                cout << brand << " " << model << " : Cannot accelerate! Engine is off" << endl;
                return;
            }
            currentSpeed += speed;
            cout << brand << " " << model << " : Accelerating to " << currentSpeed << " km/h" << endl;
        }

        void brake() {
            currentSpeed -= 20;
            if(currentSpeed < 0) currentSpeed = 0;
            cout << brand << " " << model << " : Braking! Current speed is: " << currentSpeed << " km/h" << endl;
        }

        void shiftGear(int gear) {
            if(!isEngineOn){
                cout << brand << " " << model << " : Engine is off. Turn it ON to shift the gear" << endl;
                return;
            }

            currentGear = gear;
            cout << brand << " " << model << " : Shift to grear " << currentGear << endl;
        }
};

int main() {
    ManualCar* car = new ManualCar("TATA", "Nano");

    car->startEngine();
    car->accelarate();
    car->accelarate(50);
    car->brake();
    car->stopEngine();

    return 0;
}