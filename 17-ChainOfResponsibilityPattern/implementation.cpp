/* Chain of Responsibility Pattern                                                                                                          21/06/2026 */
#include <iostream>
#include <string>
using namespace std;


// abstract handler
class MoneyHandler{
protected:
    MoneyHandler* nextHandler;

public:
    MoneyHandler() {
        this->nextHandler = nullptr;
    }

    void setNextHandler(MoneyHandler* next) {
        nextHandler = next;
    }

    virtual void dispense(int amount) = 0;
};


// concrete handlers
class ThousandHandler : public MoneyHandler {
private:
    int numNotes;

public:
    ThousandHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int needed = amount / 1000;

        if(needed > numNotes){
            needed = numNotes;
            numNotes = 0;
        } else {
            numNotes -= needed;
        }

        if(needed > 0){
            cout << "Dispensing " << needed << " x 1000rs. notes" << endl;
        }

        int remainedAmount = amount - (needed * 1000);
        if(remainedAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainedAmount);
            else cout << "Remaining amount of " << remainedAmount << " can't be fulfilled" << endl;
        }
    }
};

// concrete handlers
class FiveHundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    FiveHundredHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int needed = amount / 500;

        if(needed > numNotes){
            needed = numNotes;
            numNotes = 0;
        } else {
            numNotes -= needed;
        }

        if(needed > 0){
            cout << "Dispensing " << needed << " x 500rs. notes" << endl;
        }

        int remainedAmount = amount - (needed * 500);
        if(remainedAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainedAmount);
            else cout << "Remaining amount of " << remainedAmount << " can't be fulfilled" << endl;
        }
    }
};

// concrete handlers
class TwoHundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    TwoHundredHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int needed = amount / 200;

        if(needed > numNotes){
            needed = numNotes;
            numNotes = 0;
        } else {
            numNotes -= needed;
        }

        if(needed > 0){
            cout << "Dispensing " << needed << " x 200rs. notes" << endl;
        }

        int remainedAmount = amount - (needed * 200);
        if(remainedAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainedAmount);
            else cout << "Remaining amount of " << remainedAmount << " can't be fulfilled" << endl;
        }
    }
};

// concrete handlers
class HundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    HundredHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int needed = amount / 100;

        if(needed > numNotes){
            needed = numNotes;
            numNotes = 0;
        } else {
            numNotes -= needed;
        }

        if(needed > 0){
            cout << "Dispensing " << needed << " x 100rs. notes" << endl;
        }

        int remainedAmount = amount - (needed * 100);
        if(remainedAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainedAmount);
            else cout << "Remaining amount of " << remainedAmount << " can't be fulfilled" << endl;
        }
    }
};



// client
int main() {
    // handlers of each note
    MoneyHandler* thousandHandler = new ThousandHandler(10);
    MoneyHandler* fiveHundredHandler = new FiveHundredHandler(10);
    MoneyHandler* twoHundredHandler = new TwoHundredHandler(10);
    MoneyHandler* hundredHander = new HundredHandler(20);

    // next handlers settings
    thousandHandler->setNextHandler(fiveHundredHandler);
    fiveHundredHandler->setNextHandler(twoHundredHandler);
    twoHundredHandler->setNextHandler(hundredHander);

    int amountToBeWithdrawn = 31900;

    // init chain
    cout << "Dispensing amount: " << amountToBeWithdrawn << "rs." << endl;
    thousandHandler->dispense(amountToBeWithdrawn);

    return 0;
}