#include <iostream>
#include <vector>
#include <typeinfo>
#include <stdexcept>
using namespace std;

class DepositOnlyAccount {
    public:
        virtual void deposit(double amount) = 0;
};

class WithdrawableAccount : public DepositOnlyAccount {
    public:
        virtual void withdraw(double amount) = 0;
};

class SavingsAccount : public WithdrawableAccount {
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

class CurrentAccount: public WithdrawableAccount {
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

class FixedDepositAccount : public DepositOnlyAccount {
    private: 
        double balance;
    
    public:
        FixedDepositAccount() {
            balance = 0;
        }

        void deposit(double amount){
            balance += amount;
            cout << "Credited " << amount << " in fixed deposit account." << endl << "Updated Balance: " << balance << endl << endl;
        }        
};

class BankClient {
    private:
        vector<WithdrawableAccount*> withdrawableAccounts;
        vector<DepositOnlyAccount*> depositOnlyAccounts;

    public:
        BankClient(vector<WithdrawableAccount*> withdrawableAccounts,
        vector<DepositOnlyAccount*> depositOnlyAccounts){
            this->depositOnlyAccounts = depositOnlyAccounts;
            this->withdrawableAccounts = withdrawableAccounts;
        }

        void processTransaction() {
            for(WithdrawableAccount* acc : withdrawableAccounts){
                acc->deposit(1500);
                acc->withdraw(800);
            }

            for(DepositOnlyAccount* acc: depositOnlyAccounts){
                acc->deposit(1600);
            }
        }
};

int main() {
    vector<WithdrawableAccount*> WithdrawableAccounts;
    WithdrawableAccounts.push_back(new SavingsAccount());
    WithdrawableAccounts.push_back(new CurrentAccount());

    vector<DepositOnlyAccount*> DepositOnlyAccounts;
    DepositOnlyAccounts.push_back(new FixedDepositAccount());

    BankClient* cli = new BankClient(WithdrawableAccounts, DepositOnlyAccounts);
    cli->processTransaction();
    
    return 0;
}
