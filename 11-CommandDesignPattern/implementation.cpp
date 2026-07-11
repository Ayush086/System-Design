/*  COMMEND DESIGN PATTERN                                                                                                                  24/05/2026  */
#include <iostream>
using namespace std;

// command abstract class
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() {}
};

// Receivers 
class Light {
public:
    void on() {
        cout << "Light is turned ON" << endl;
    }

    void off() {
        cout << "Light is turned OFF" << endl;
    }
};

class Fan {
public:
    void on() {
        cout << "Fan is turned ON" << endl;
    }

    void off() {
        cout << "Fan is turned OFF" << endl;
    }
};


// concrete class : Light command
class LightCommand : public ICommand {
private:
    Light* light;

public:
    LightCommand(Light* l) {
        this->light = l;
    }

    void execute() {
        light->on();
    }

    void undo() {
        light->off();
    }
};

// concrete class : Fan command
class FanCommand : public ICommand {
private:
    Fan* fan;

public: 
    FanCommand(Fan* f){
        this->fan = f;
    }

    void execute() {
        fan->on();
    }

    void undo() {
        fan->off();
    }
};


// Remote controller to access the receivers (commands)
class RemoteController {
private:
    static const int numButtons = 4;
    ICommand* buttons[numButtons];
    bool buttonPressed[numButtons];
public:
    RemoteController() {
        // setting all the buttons to be OFF
        for (int i = 0; i < numButtons; i++) {
            buttons[i] = nullptr;
            buttonPressed[i] = false;
        }
    }

    void setCommand(int idx, ICommand* cmd) {
        if(idx >= 0 && idx < numButtons) {
            // if already any command is set, then delete it
            if(buttons[idx] != nullptr){
                delete buttons[idx];
            }
            // set new command
            buttons[idx] = cmd;
            buttonPressed[idx] = false;
        }
    }

    void pressButton(int idx) {
        if(idx >= 0 && idx < numButtons && buttons[idx] != nullptr) {
            if(buttonPressed[idx] == false){
                buttons[idx]->execute();
            } else {
                buttons[idx]->undo();
            }
            buttonPressed[idx] = !buttonPressed[idx];
        } else {
            cout << "No command assigned at button " << idx << endl;
        }
    }

    ~RemoteController() {
        for(int i = 0; i < numButtons; i++) {
            if(buttons[i] != NULL) delete buttons[i];
        }
    }
};


int main(){
    Light* livingRoomLight = new Light();
    Fan* ceilingFan = new Fan();

    RemoteController* remote = new RemoteController();

    remote->setCommand(0, new LightCommand(livingRoomLight));
    remote->setCommand(1, new FanCommand(ceilingFan));

    // simute button press for light
    cout << "---- Light's button ----" << endl;
    remote->pressButton(0); // on
    remote->pressButton(0); // off

    cout << "---- Fan's button ----" << endl;
    remote->pressButton(1); // on
    remote->pressButton(1); // off

    // unassigned button press
    remote->pressButton(2);

    delete remote;
    delete livingRoomLight;
    delete ceilingFan;

    return 0;
}















