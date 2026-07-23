/* State Design Pattern                                                                                     Date: 23/07/2026 */
#include <iostream>
#include <string>
using namespace std;

// forward declaration
class VendingMachine;

// concrete states forward declaration
class NoCoinState;
class HasCoinState;
class DispenseState;
class SoldOutState;

class VendingState {
public:
    virtual VendingState* insertCoin(VendingMachine* machine, int coin) = 0;
    virtual VendingState* selectItem(VendingMachine* machine) = 0;
    virtual VendingState* dispense(VendingMachine* machine) = 0;
    virtual VendingState* returnCoin(VendingMachine* machine) = 0;
    virtual VendingState* refill(VendingMachine* machine, int quantity) = 0;
    virtual string getStateName() = 0;
};

// vending machine - context class
class VendingMachine {
private:
    VendingState* currentState;
    int itemCount;
    int itemPrice;
    int insertedCoins;

    // state objects
    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* dispenseState;
    VendingState* soldOutState;

public:
    VendingMachine(int itemCount, int itemPrice);

    void insertCoin(int coin);
    void selectItem();
    void dispense();
    void returnCoin();
    void refill(int qty);

    void printStatus();

    VendingState* getNoCoinState() {
        return noCoinState;
    }

    VendingState* getHasCoinState() {
        return hasCoinState;
    }

    VendingState* getDispenseState() {
        return dispenseState;
    }

    VendingState* getSoldOutState() {
        return soldOutState;
    }

    int getItemCount() {
        return this->itemCount;
    }

    void decrementItemCount() {
        this->itemCount--;
    }

    void incrementItemCount(int count = 1) {
        this->itemCount += count;
    }

    int getInsertedCoin() {
        return this->insertedCoins;
    }

    void setInsertedCoin(int coin) {
        this->insertedCoins = coin;
    }

    void addCoin(int coin) {
        this->insertedCoins += coin;
    }

    int getPrice() {
        return this->itemPrice;
    }

    void setPrice(int itemPrice) {
        this->itemPrice = itemPrice;
    }
};

// concrete states
class NoCoinState : public VendingState{
public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        machine->setInsertedCoin(coin);
        cout << "Coin inserted. Current Balance: Rs. " << coin << endl;
        return machine->getHasCoinState(); // move to next state
    }

    VendingState* selectItem(VendingMachine* machine) override {
        cout << "Please insert coin first!" << endl;
        return machine->getNoCoinState(); // stick to same state
    }

    VendingState* dispense(VendingMachine* machine) override {
        cout << "Please insert coin and select the item" << endl;
        return machine->getNoCoinState(); // same state
    }

    VendingState* returnCoin(VendingMachine* machine) override {
        cout << "No coin to return. " << endl;
        return machine->getNoCoinState(); // again same state
    }

    VendingState* refill(VendingMachine* machine, int qty) override {
        cout << "Items refilling" << endl;
        machine->incrementItemCount(qty);
        return machine->getNoCoinState();
    }

    string getStateName() override {
        return "NO-COIN";
    }
};

class HasCoinState : public VendingState{
public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        machine->addCoin(coin);
        cout << "Additonal coin inserted. Current Balance: Rs. " << machine->getInsertedCoin() << endl;
        return machine->getHasCoinState();
    }

    VendingState* selectItem(VendingMachine* machine) override {
        if(machine->getInsertedCoin() >= machine->getPrice()) {
            cout << "Item selected. Dispensing..." << endl;

            int change = machine->getInsertedCoin() - machine->getPrice();
            if(change > 0) {
                cout << "Change returned: Rs. " << change << endl;
            }
            machine->setInsertedCoin(0);
    
            return machine->getDispenseState();
        } else {
            int needed = machine->getPrice() - machine->getInsertedCoin();
            cout << "Insufficient balance. Need more: Rs. " << needed << endl;
            return machine->getHasCoinState();
        }
    }

    VendingState* dispense(VendingMachine* machine) override {
        cout << "Please select the item" << endl;
        return machine->getHasCoinState();
    }

    VendingState* returnCoin(VendingMachine* machine) override {
        cout << "Please collect your returned coin: Rs " << machine->getInsertedCoin() << endl;
        machine->setInsertedCoin(0);
        return machine->getNoCoinState();
    }

    VendingState* refill(VendingMachine* machine, int qty) override {
        cout << "Can't refil in this state" << endl;
        return machine->getHasCoinState();
    }

    string getStateName() override {
        return "HAS-COIN";
    }
};

class DispenseState : public VendingState{
public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        cout << "Can't insert more coins now"<< endl;
        return machine->getDispenseState();
    }

    VendingState* selectItem(VendingMachine* machine) override {
        cout << "Already dispensing item. wait" << endl;
        return machine->getDispenseState();
    }

    VendingState* dispense(VendingMachine* machine) override {
        cout << "Item Dispensed!" << endl;
        machine->decrementItemCount();
        
        if(machine->getItemCount() > 0) return machine->getNoCoinState();
        else {
            cout << "machine is now sold out" << endl;
            return machine->getSoldOutState();
        }
    }

    VendingState* returnCoin(VendingMachine* machine) override {
        cout << "Can't return coin while dispensing item" << endl;
        return machine->getDispenseState();
    }

    VendingState* refill(VendingMachine* machine, int qty) override {
        cout << "Can't refil in this state" << endl;
        return machine->getDispenseState();
    }

    string getStateName() override {
        return "DISPENSE";
    }
};

class SoldOutState : public VendingState{
public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        cout << "machine sold out. Coin returned: Rs " << coin << endl;
        return machine->getSoldOutState();
    }

    VendingState* selectItem(VendingMachine* machine) override {
        cout << "machine sold out" << endl;
        return machine->getSoldOutState();
    }

    VendingState* dispense(VendingMachine* machine) override {
        cout << "Machine sold out" << endl;
        return machine->getSoldOutState();
    }

    VendingState* returnCoin(VendingMachine* machine) override {
        cout << "machine sold out. no coin inserted" << endl;
        return machine->getSoldOutState();
    }

    VendingState* refill(VendingMachine* machine, int qty) override {
        cout << "Items refilling" << endl;
        machine->incrementItemCount(qty);
        return machine->getNoCoinState();
    }

    string getStateName() override {
        return "SOLD-OUT";
    }
};


// implementation
VendingMachine::VendingMachine(int itemCount, int itemPrice) {
    this->itemCount = itemCount;
    this->itemPrice = itemPrice;
    this->insertedCoins = 0;

    noCoinState = new NoCoinState();
    hasCoinState = new HasCoinState();
    dispenseState = new DispenseState();
    soldOutState = new SoldOutState();

    if(itemCount > 0) {
        this->currentState = noCoinState;
    } else {
        this->currentState = soldOutState;
    }
}

void VendingMachine::insertCoin(int coin) {
    this->currentState = this->currentState->insertCoin(this, coin);
}

void VendingMachine::selectItem() {
    this->currentState = this->currentState->selectItem(this);
}

void VendingMachine::dispense() {
    this->currentState = this->currentState->dispense(this);
}

void VendingMachine::returnCoin() {
    this->currentState = this->currentState->returnCoin(this);
}

void VendingMachine::refill(int qty) {
    this->currentState = this->currentState->refill(this, qty);
}

void VendingMachine::printStatus() {
    cout << endl << "------- Vending Machine Status ------" << endl;
    cout << "Items remaining: " << this->itemCount << endl;
    cout << "Inserted coin: Rs. " << this->insertedCoins << endl;
    cout << "Current state: " << currentState->getStateName() << endl << endl;
}


int main() {
    cout << "==== Water bottle vending machine ====" << endl;

    int itemCount = 2;
    int itemPrice = 20;

    VendingMachine machine(itemCount, itemPrice);
    machine.printStatus();

    cout << "Case 1: trying to select item without coin" << endl;
    machine.selectItem();
    machine.printStatus();

    cout << "Case 2: inserting coin" << endl;
    machine.insertCoin(10);
    machine.printStatus();
    
    cout << "Case 3: selecting item with insufficient balance" << endl;
    machine.selectItem();
    machine.printStatus();
    
    cout << "Case 4: inserting more coin" << endl;
    machine.insertCoin(10);
    machine.printStatus();

    cout << "Case 5: seleting item now" << endl;
    machine.selectItem();
    machine.printStatus();

    cout << "Case 6: Dispening item" << endl;
    machine.dispense();
    machine.printStatus();

    cout << "Case 7: Buying last item" << endl;
    machine.insertCoin(20);
    machine.selectItem();
    machine.dispense();
    machine.printStatus();

    cout << "Case 8: trying to use sold out machine" << endl;
    machine.insertCoin(5);
    machine.printStatus();

    cout << "Case 9: trying to refill sold out machine" << endl;
    machine.refill(2);
    machine.printStatus();

    return 0;
}