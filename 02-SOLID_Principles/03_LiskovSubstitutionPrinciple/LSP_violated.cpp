#include <iostream>
#include <vector>
using namespace std;

// abstract class
class Account {
    public:
        virtual void deposit(double amount) = 0;
        virtual void withdraw(double amount) = 0;
};

class SavingsAccount : public Account {
    private:
        double balance;

    public:
        SavingsAccount() {
            balance = 0;
        }

        void deposit(double amount){
            balance += amount;
            cout << "Credited " << amount << " in savings account." << endl << "Updated Balance: " << balance << endl << endl;
        }

        void withdraw(double amount){
            if(balance >= amount){
                balance -= amount;
                cout << "Withdrawn " << amount << " from savings account. " << endl << "Updated Balance: " << balance << endl << endl;
            } else {
                cout << "Insufficient Balance" << endl;
            }
        }
};

class CurrentAccount : public Account {
    private:
        double balance;

    public:
        CurrentAccount() {
            balance = 0;
        }

        void deposit(double amount){
            balance += amount;
            cout << "Credited " << amount << " in current account." << endl << "Updated Balance: " << balance << endl << endl;
        }

        void withdraw(double amount){
            if(balance >= amount){
                balance -= amount;
                cout << "Withdrawn " << amount << " from current account. " << endl << "Updated Balance: " << balance << endl << endl;
            } else {
                cout << "Insufficient Balance" << endl;
            }
        }
};

class FixedDepositAccount : public Account {
    private:
        double balance;

    public:
        FixedDepositAccount() {
            balance = 0;
        }

        void deposit(double amount){
            balance += amount;
            cout << "Credited " << amount << " in Fixed Deposit account." << endl << "Updated Balance: " << balance << endl << endl;
        }

        void withdraw(double amount){
            throw logic_error("Withdrawal not allowed in Fixed Deposit account.");
        }
};


class BankClient {
    private: 
        vector<Account*> accounts;

    public:
        BankClient(vector<Account*> accounts) {
            this->accounts = accounts;
        }

        // void processTransactions() {
        //     for(Account* acc: accounts){
        //         acc->deposit(1000); // deposit is allwed for all type of accounts

        //         // assuming all accounts supports withdrawal
        //         try
        //         {
        //             acc->withdraw(500);
        //         }
        //         catch(const logic_error& e)
        //         {
        //             cout << "Exception: " << e.what() << endl;
        //         }
                
        //     }
        // }

        // WRONG SOLUTION: it's a way to prevent it but again it's not recommended as it's makes us to make the changes in client class which will make code tightly coupled.
        void processTransactions() {
            for(Account* acc: accounts){
                acc->deposit(1000); // deposit is allwed for all type of accounts

                // checking account type 
                if(typeid(*acc) == typeid(FixedDepositAccount)) {
                    cout << "Withdrwal can't be processed. Since it's a Fixed Deposit Account." << endl;
                } else {
                    try {
                        acc->withdraw(500);
                    }
                    catch(const logic_error& e) {
                        cout << "Exception: " << e.what() << endl;
                    }
                }
                
            }
        }
};

/*
NOTE: this way of logic handling is not efficient what if in future new account gets introduced where deposit is prohibitted then we will need to make changes in 'BankClient' Class which is not advisable and code will be tightly coupled
*/

int main(int argc, char const *argv[])
{
    vector<Account*> accounts;
    accounts.push_back(new SavingsAccount());
    accounts.push_back(new CurrentAccount());
    accounts.push_back(new FixedDepositAccount());

    BankClient* client = new BankClient(accounts);
    client->processTransactions(); // will throw exception when accountType -> fixed and withdraw method is called
    return 0;
}
