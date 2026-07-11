#include <iostream>
#include <string>
using namespace std;

class SportsCar {
    private:
        string brand;
        string model;
        bool isEngineOn;
        int currentSpeed;
        int currentGear;
        string tyre;

    public:
        // constructor
        SportsCar(string b, string m){
            this->brand = b;
            this->model = m;
            isEngineOn = false;
            currentSpeed = 0;
            currentGear = 0;
            tyre = "MRF";
        }

        // getter and setter - used to read and modify the variables which are set to private
        int getCurrentSpeed(){
            return this->currentSpeed;
        }

        string getTyre(){
            return this->tyre;
        }

        void setTyre(string t){
            // setter is usually used to perform validation before the changing the current value
            this->tyre = t;
            return;
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

        // destructor
        virtual ~SportsCar(){}
};

int main() {
    SportsCar* newCar = new SportsCar("Ferrari", "Enzo");

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

    // after changing the access modifier it will become inaccessible but they must be accessible to read not write
    // newCar->currentSpeed = 200; // setting speed manually which should not be allowed

    cout << "current spped is now set to: " << newCar->getCurrentSpeed() << endl;

    delete newCar;
    
    return 0;
}
