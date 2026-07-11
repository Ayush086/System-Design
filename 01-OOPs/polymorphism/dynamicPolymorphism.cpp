#include <iostream>
#include <string>
using namespace std;

class Car{
    protected:
        string brand;
        string model;
        bool isEngineOn;
        int currentSpeed;

    public:
        Car(string a, string m){
            this->brand = a;
            this->model = m;
            isEngineOn = false;
            currentSpeed = 0;
        }

        void startEngine(){
            isEngineOn = true;
            cout << brand << " " << model << " : Engine started." << endl;
        }

        void stopEngine() {
            isEngineOn = false;
            currentSpeed = 0;
            cout << brand << " " << model << " : Engine turned OFF. " << endl;
        }

        virtual void accelarate() = 0;
        virtual void brake() = 0;
        virtual ~Car() {}
};

class ManualCar : public Car { // inheriting from Car class as public
    private:
        int currentGear; // it's specific to this class

    public:
        ManualCar(string b, string m) : Car(b, m){
            this->currentGear = 0;
        }

        // specific method
        void shiftGear(int gear){
            currentGear = gear;
            cout << brand << " " << model << " : Shifted to gear " << currentGear << endl;
        }

        // overriding accelarate() - Dynamic Polymorphism
        void accelarate() {
            if(!isEngineOn){
                cout << brand << " " << model << " : Engine is off. Turn it ON to accelarate" << endl;
                return;
            }

            currentSpeed += 20;
            cout << brand << " " << model << " : Accelarating to " << currentSpeed << " km/h" << endl;
        }

        // o verriding brake() - Dynamic Polymorphism
        void brake() {
            currentSpeed -= 20;
            if(currentSpeed < 0) currentSpeed = 0;
            cout << brand << " " << model << " : Braking! Current speed is: " << currentSpeed << " km/h" << endl;
        }
};

class ElectricCar : public Car {
    private:
        int batteryLevel;

    public:
        ElectricCar(string b, string m) : Car(b, m){
            batteryLevel = 100;
        }

        void chargeBattery() {
            batteryLevel = 100;
            cout << brand << " " << model << " : Battery fully charged." << endl;
        }

        // overriding accelarate() - Dynamic Polymorphism
        void accelarate() {
            if(!isEngineOn){
                cout << brand << " " << model << " : Engine is off. Turn it ON to accelarate" << endl;
                return;
            }

            currentSpeed += 15;
            batteryLevel -= 10;
            cout << brand << " " << model << " : Accelarating to " << currentSpeed << " km/h. Battery at " << batteryLevel << "%" << endl;
        }

        // o verriding brake() - Dynamic Polymorphism
        void brake() {
            currentSpeed -= 15;
            if(currentSpeed < 0) currentSpeed = 0;
            cout << brand << " " << model << " : Braking! Current speed is: " << currentSpeed << " km/h" << endl;
        }
};

int main(int argc, char const *argv[])
{
    ManualCar* myManual = new ManualCar("TATA", "Nano");
    myManual->startEngine();
    myManual->shiftGear(1); // specific method call
    myManual->accelarate();
    myManual->accelarate();
    myManual->accelarate();
    myManual->shiftGear(2);
    myManual->brake();
    myManual->shiftGear(1);
    myManual->stopEngine();

    cout << " --------------------------------------------- " << endl;

    ElectricCar* myElectric = new ElectricCar("TATA", "Nexon");
    myElectric->chargeBattery(); // specific to this class
    myElectric->startEngine();
    myElectric->accelarate();
    myElectric->accelarate();
    myElectric->brake();
    myElectric->stopEngine();

    // delete the object
    delete myManual;
    delete myElectric;
    return 0;
}

