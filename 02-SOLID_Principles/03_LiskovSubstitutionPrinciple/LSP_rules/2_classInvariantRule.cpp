#include <iostream>
using namespace std;

// Invariant : balance can't be negative
class BankAccount {
protected:
    double balance;
public:
    BankAccount(double b){
        if (b < 0) throw invalid_argument("Balance can't be negative");
        balance = b;
    }

    virtual void withdraw(double amount) {
        if (balance - amount < 0) throw runtime_error("Insufficient Balance");
        balance -= amount;
        cout << "Amount: " << amount << " withdrawn successfully. Remaining balance: " << balance << endl;
    }
};

// breaks invariant : should not be allowed
class CheatAccount : public BankAccount{
    public:
        CheatAccount(double b) : BankAccount(b) {}

        void withdraw(double amount) override {
            balance -= amount;
            cout << "Amount: " << amount << " withdrawn successfully. Remaining balance: " << balance << endl;
        }
};

int main() {
    // BankAccount* bankAccount = new CheatAccount(100);
    BankAccount* bankAccount = new BankAccount(100);
    bankAccount->withdraw(101);
}