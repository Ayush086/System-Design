/* Expense Splitting Application                                                                              Date: 22/07/2026 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

// forward declerations
class User;
class Group;
class ExpenseManager;

enum class SplitTypes {
    EQUAL,
    EXACT,
    PERCENTAGE
};

class Split {
public:
    string userId;
    double amount;

    Split(const string id, double amount) {
        this->userId = id;
        this->amount = amount;
    }
};

// notification - observer pattern
class Observer {
public:
    virtual void update(const string& message) = 0;
};

// Split strategies (strategy pattern)
class SplitStrategy {
public:
    virtual vector<Split> calculateSplit(double totalAmount, const vector<string>& userIds, const vector<double>& values = {}) = 0;
};

class EqualSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount, const vector<string>& userIds, const vector<double>& values = {}) override {
        vector<Split> splits;
        double amountPerUser = totalAmount / userIds.size();
        
        for (const string& uid: userIds) {
            splits.push_back(Split(uid, amountPerUser));
        }
        
        return splits;
    }
    
};

class ExactSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount, const vector<string>& userIds, const vector<double>& values = {}) override {
        // validations
        if(userIds.size() != values.size()) {
            cout << "Either users are more than split values or vice versa" << endl;
            return {};
        }

        vector<Split> splits;

        for(int i = 0; i < userIds.size(); i++) {
            splits.push_back(Split(userIds[i], values[i]));
        }

        return splits;
    }
};

class PercentageSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount, const vector<string>& userIds, const vector<double>& values = {}) override {
        vector<Split> splits;
    
        for(int i = 0; i < userIds.size(); i++) {
            splits.push_back(Split(userIds[i], totalAmount*(values[i]/100)));
        }
    
        return splits;
    }
};

// Split strategies factory
class SplitFactory {
public:
    static SplitStrategy* getSplitStrategy(SplitTypes type) {
        switch(type){
            case SplitTypes::EQUAL:
                return new EqualSplit();
            case SplitTypes::EXACT:
                return new ExactSplit();
            case SplitTypes::PERCENTAGE:
                return new PercentageSplit();
            default:
                return new EqualSplit();
        }
    }
};


// user - concrete observer
class User : public Observer {
public:
    static int nextUserId;
    string userId;
    string name;
    string email;
    map<string, double> balances;

    User(const string& name, const string& email) {
        this->userId = "user" + to_string(++nextUserId);
        this->name = name;
        this->email = email;
    }

    void update(const string& message) override {
        cout << "[NOTIFICATION to " << name << "]: " << message << endl;
    }

    void updateBalance(const string& otherUserId, double amount) {
        this->balances[otherUserId] += amount;

        if(std::abs(balances[otherUserId]) < 0.01) {
            balances.erase(otherUserId);
        }
    }

    double getTotalOwed() {
        double total = 0;
        for(const auto& balance: balances) {
            if(balance.second < 0) {
                total += abs(balance.second);
            }
        }

        return total;
    }

    double getTotalOwing() {
        double total = 0;
        for(const auto& balance: balances) {
            if(balance.second > 0) {
                total += abs(balance.second);
            }
        }

        return total;
    }
};
// static member definition
int User::nextUserId = 0;

// model class 
class Expense{
public:
    static int nextExpenseId;
    string expenseId;
    string description;
    double totalAmount;
    string paidByUserId;
    vector<Split> splits;
    string groupId;

    Expense(const string& desc, double amount, const string& paidBy,
            vector<Split>& splits, const string group="") {
                this->expenseId = "expense" + to_string(++nextExpenseId);
                this->description = desc;
                this->totalAmount = amount;
                this->paidByUserId = paidBy;
                this->splits = splits;
                this->groupId = group;
            }
};
int Expense::nextExpenseId = 0;

class DebtSimplifier {
public:
    static map<string, map<string, double>> simplifyDebts(
        map<string, map<string, double>> groupBalances) {
        
        // Calculate net amount for each person
        map<string, double> netAmounts;
        
        // Initialize all users with 0
        for (const auto& userBalance : groupBalances) {
            netAmounts[userBalance.first] = 0;
        }
        
        // Calculate net amounts
        // We only need to process each balance once (not twice)
        // If groupBalances[A][B] = 200, it means B owes A 200
        // So A should receive 200 (positive) and B should pay 200 (negative)
        for (const auto& userBalance : groupBalances) {
            string creditorId = userBalance.first;
            for (const auto& balance : userBalance.second) {
                string debtorId = balance.first;
                double amount = balance.second;
                
                // Only process positive amounts to avoid double counting
                if (amount > 0) {
                    netAmounts[creditorId] += amount;  // creditor receives
                    netAmounts[debtorId] -= amount;    // debtor pays
                }
            }
        }
        
        // Divide users into creditors and debtors
        vector<pair<string, double>> creditors; // those who should receive money
        vector<pair<string, double>> debtors;   // those who should pay money
        
        for (const auto& net : netAmounts) {
            if (net.second > 0.01) { // creditor
                creditors.push_back({net.first, net.second});
            } else if (net.second < -0.01) { // debtor
                debtors.push_back({net.first, -net.second}); // store positive amount
            }
        }
        
        // Sort for better optimization (largest amounts first)
        sort(creditors.begin(), creditors.end(), 
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });
        sort(debtors.begin(), debtors.end(), 
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });
        
        // Create new simplified balance map
        map<string, map<string, double>> simplifiedBalances;
        
        // Initialize empty maps for all users
        for (const auto& userBalance : groupBalances) {
            simplifiedBalances[userBalance.first] = map<string, double>();
        }
        
        // Use greedy algorithm to minimize transactions
        int i = 0, j = 0;
        while (i < creditors.size() && j < debtors.size()) {
            string creditorId = creditors[i].first;
            string debtorId = debtors[j].first;
            double creditorAmount = creditors[i].second;
            double debtorAmount = debtors[j].second;
            
            // Find the minimum amount to settle
            double settleAmount = min(creditorAmount, debtorAmount);
            
            // Update simplified balances
            // debtorId owes creditorId the settleAmount
            simplifiedBalances[creditorId][debtorId] = settleAmount;
            simplifiedBalances[debtorId][creditorId] = -settleAmount;
            
            // Update remaining amounts
            creditors[i].second -= settleAmount;
            debtors[j].second -= settleAmount;
            
            // Move to next creditor or debtor if current one is settled
            if (creditors[i].second < 0.01) {
                i++;
            }
            if (debtors[j].second < 0.01) {
                j++;
            }
        }
        
        return simplifiedBalances;
    }
};

// group (concrete observable)
class Group {
private:
    User* getUserByuserId(string uid) {
        User* user = nullptr;

        for(User* member: members) {
            if(member->userId == uid) user = member;
        }

        return user;
    }

public:
    static int nextGroupId;
    string groupId;
    string name;
    vector<User*> members; // observers
    map<string, Expense*> groupExpenses; // group's expense book
    map<string, map<string, double>> groupBalances; // userId -> {otherUserId -> balance}

    Group(const string& name) {
        this->groupId = "group" + to_string(++nextGroupId);
        this->name = name;
    }

    void addMember(User* user){
        members.push_back(user);

        // init balance map for new user
        groupBalances[user->userId] = map<string, double>();
        cout << user->name << " added to group " << this->name << endl;
        return;
    }

    bool removeMember(const string& uid) {
        // check eligibility
        if(!canUserLeaveGroup(uid)) {
            cout << endl << "User is not allowed to leave group without clearing all expenses" << endl;
            return false;
        }

        // remove from observers
        for(User* user: members) {
            if(user->userId == uid) {
                members.erase(remove(members.begin(), members.end(), user), members.end());
                break;
            }
        }

        // remove from group balances
        groupBalances.erase(uid);

        // remove this user form other members balance sheets
        for(auto& memberBalance: groupBalances) {
            memberBalance.second.erase(uid);
        }

        return true;
    }

    void notifyMembers(const string& message) {
        for(Observer* observer : members) {
            observer->update(message);
        }
    }

    bool isMember(const string& uid) {
        return groupBalances.find(uid) != groupBalances.end();
    }

    // update balance within group
    void updateGroupBalance(const string& fromUserId, const string& toUserId, double amount) {
        groupBalances[fromUserId][toUserId] += amount;
        groupBalances[toUserId][fromUserId] -= amount;

        // remove if balance = 0; (settlement)
        if(std::abs(groupBalances[fromUserId][toUserId]) < 0.01){
            groupBalances[fromUserId].erase(toUserId);
        }
        if(std::abs(groupBalances[toUserId][fromUserId]) < 0.01){
            groupBalances[toUserId].erase(fromUserId);
        }
    }

    bool canUserLeaveGroup(const string& uid) {
        if(!isMember(uid)) {
            throw runtime_error("user is not a part of this group");
        }

        // check if user has any settlement remained
        map<string, double> userBalanceSheet = groupBalances[uid];
        for(auto& balance : userBalanceSheet) {
            if(std::abs(balance.second) > 0.01) {
                return false;
            }
        }

        return true;
    }

    map<string, double> getUserGroupBalances(const string& uid) {
        if(!isMember(uid)) throw runtime_error("user is not a part of this group");

        return groupBalances[uid];
    }

    // add expense to group
    bool addExpense(string& desc, double amt, string& paidByUserId,
        vector<string>& involvedUsers, SplitTypes type, const vector<double>& splitValues = {}){
        if(!isMember(paidByUserId)){
            throw runtime_error("user is not a part of this group");
        }

        // validate all involved users are group members
        for(const string& uid: involvedUsers) {
            if(!isMember(uid)) throw runtime_error("user is not a part of this group");
        }

        // generate splits based on it's types
        vector<Split> splits = SplitFactory::getSplitStrategy(type)->calculateSplit(amt, involvedUsers, splitValues);

        // create expense in group's expense book
        Expense* expense = new Expense(desc, amt, paidByUserId, splits, groupId);
        groupExpenses[expense->expenseId] = expense;

        // update group balance
        for(Split& split: splits) {
            if(split.userId != paidByUserId) {
                updateGroupBalance(paidByUserId, split.userId, split.amount);
            }
        }

        cout << endl << "------------- Sending Notifications ---------------" << endl;
        string paidByName = getUserByuserId(paidByUserId)->name;
        notifyMembers("New expense added: " + desc + " (Rs. " + to_string(amt) + ")");
        
        cout << endl << "------------- Expense Message ---------------" << endl;
        cout << "Expense added to " << name << ": " << desc << " (Rs. " << amt <<
        ") paid by " << paidByName <<" and involved people are: " << endl;
        if(!splitValues.empty()) {
            for(int i=0; i<splitValues.size(); i++) {
                cout << getUserByuserId(involvedUsers[i])->name << " : " << splitValues[i] << endl;
            }
        } 
        else {
            for(string user : involvedUsers) {
                cout << getUserByuserId(user)->name << ", ";
            }
            cout << endl << "Will be Paid Equally" << endl;
        }
        
        return true;
    }

    bool settlePayment(string& fromUserId, string& toUserId, double amount) {
        // validate both users are group members
        if(!isMember(fromUserId) || !isMember(toUserId)) {
            cout << "user is not a part of this group" << endl;
            return false;
        }

        // update group balance
        updateGroupBalance(fromUserId, toUserId, amount);   

        // get names
        string fromName = getUserByuserId(fromUserId)->name;
        string toName = getUserByuserId(toUserId)->name;

        // notify all
        notifyMembers("Settlement: " + fromName + " paid " + toName + "Rs. " + to_string(amount));

        cout << "Settlement in " << name << ": " << fromName << " settled Rs. " << amount << " with " << toName << endl;

        return true;
    }

    void showGroupBalances() {
        cout << endl << "=== Group Balances for " << name << " ===" << endl;
        
        for (const auto& pair : groupBalances) {
            string memberId = pair.first;
            string memberName = getUserByuserId(memberId)->name;

            cout << memberName << "'s balances in group:" << endl;
            
            auto userBalances = pair.second;
            if (userBalances.empty()) {
                cout << "  No outstanding balances" << endl;
            } 
            else {
                for (const auto& userBalance : userBalances) {
                    string otherMemberUserId = userBalance.first;
                    string otherName = getUserByuserId(otherMemberUserId)->name;
                    
                    double balance = userBalance.second;
                    if (balance > 0) {
                        cout << "  " << otherName << " owes: Rs " << fixed << setprecision(2) << balance << endl;
                    } else {
                        cout << "  Owes " << otherName << ": Rs " << fixed << setprecision(2) << std::abs(balance) << endl;
                    }
                }
            }
        }
    }

    void simplifyGroupDebts() {
        map<string, map<string, double>> simplifiedBalances = DebtSimplifier::simplifyDebts(groupBalances);
        groupBalances = simplifiedBalances;

        cout << endl << "Debts have been simplified for group: " << this->name << endl;
    }

    ~Group() {
        for(auto& pair: groupExpenses){
            delete pair.second;
        }
    }
};
int Group::nextGroupId = 0;


// main manager (singleton + facade)
class Splitwise {
private:
    map<string, User*> users;
    map<string, Group*> groups;
    map<string, Expense*> expenses;

    static Splitwise* instance;
    Splitwise() {}

public:
    static Splitwise* getInstance() {
        if(instance == nullptr) {
            instance = new Splitwise();
        }

        return instance;
    }

    // User Management
    User* createUser(string name, string email) {
        User* user = new User(name, email);
        users[user->userId] = user;
        cout << "User created: " << name << "(ID: " << user->userId << ")" << endl;
        return user;
    }
    
    User* getUser(const string& userid) {
        auto it = users.find(userid);
        return (it != users.end()) ? it->second : nullptr;
    }
    
    // Group Management
    Group* createGroup(const string name) {
        Group* group = new Group(name);
        groups[group->groupId] = group;
        cout << "Group created: " << name << "(ID: " << group->groupId << ")" << endl;
        return group;
    }

    Group* getGroup(const string& groupid){
        auto it = groups.find(groupid);
        return (it != groups.end()) ? it->second : nullptr;
    }

    void addUserToGroup(const string& userId, const string& groupId) {
        User* user = getUser(userId);
        Group* group = getGroup(groupId);

        if(user && group) {
            group->addMember(user);
        }
    }

    bool removeUserFromGroup(const string& userid, const string& gid) {
        Group* group = getGroup(gid);

        if(!group) {
            cout << "Group not found!" << endl;
            return false;
        }

        User* user = getUser(userid);

        if(!user) {
            cout << "User not found!" << endl;
            return false;
        }

        bool isRemoved = group->removeMember(userid);
        
        if(isRemoved) {
            cout << user->name << " successfully left " << group->name << endl;
        } 
        
        return isRemoved;
    }

    // Expense Management
    void addExpenseToGroup(string& groupId, string description, double amount, 
                          string& paidByUserId, vector<string>& involvedUsers, 
                          SplitTypes splitType, const vector<double>& splitValues = {}) {
        Group* group = getGroup(groupId);
        if (!group) {
            cout << "Group not found!" << endl;
            return;
        }
        
        group->addExpense(description, amount, paidByUserId, involvedUsers, splitType, splitValues);
    }

    void settlePaymentInGroup(string& groupId, string& fromUserId, 
                              string& toUserId, double amount) {
        Group* group = getGroup(groupId);
        if (!group) {
            cout << "Group not found!" << endl;
            return;
        }
        
        group->settlePayment(fromUserId, toUserId, amount);
    }

    // individual settlement
    void settleIndividualPayment(string& fromUserId, string& toUserId, double amount) {
        User* fromUser = getUser(fromUserId);
        User* toUser = getUser(toUserId);

        if(fromUser && toUser) {
            fromUser->updateBalance(toUserId, amount);
            toUser->updateBalance(fromUserId, -amount);

            cout << fromUser->name << " settle Rs. " << amount << " with " << toUser->name << endl;
        }
    }

    void addIndividualExpense(string description, double amount, string paidByUserId,
                             string toUserId, SplitTypes splitType,
                            const vector<double>& splitValues = {}) {

        SplitStrategy* strategy = SplitFactory::getSplitStrategy(splitType);
        vector<Split> splits = strategy->calculateSplit(amount, {paidByUserId, toUserId}, splitValues);

        Expense* expense = new Expense(description, amount, paidByUserId, splits);
        expenses[expense->expenseId] = expense;
        
        User* paidByUser = getUser(paidByUserId);
        User* toUser = getUser(toUserId);

        paidByUser->updateBalance(toUserId, amount);
        toUser->updateBalance(paidByUserId, -amount);
        
        cout << "Individual expense added: " << description << " (Rs " << amount 
                << ") paid by " << paidByUser->name <<" for " << toUser->name << endl;
    }

    void showUserBalance(string& userId) {
        User* user = getUser(userId);
        if (!user) return;
        
        cout << endl << "=========== Balance for " << user->name <<" ===================="<<endl; 
        cout << "Total you owe: Rs " << fixed << setprecision(2) << user->getTotalOwed() << endl;
        cout << "Total others owe you: Rs " << fixed << setprecision(2) << user->getTotalOwing() << endl;
        
        cout << "Detailed balances:" << endl;
        for (auto& balance : user->balances) {
            User* otherUser = getUser(balance.first);
            if (otherUser) {
                if (balance.second > 0) {
                    cout << "  " << otherUser->name << " owes you: Rs" << balance.second << endl;
                } else {
                    cout << "  You owe " << otherUser->name << ": Rs" << std::abs(balance.second) << endl;
                }
            }
        }
    }

    void showGroupBalances(string& gid) {
        Group* group = getGroup(gid);
        if(!group) return;

        group->showGroupBalances();
    }

    void simplifyGroupDebts(string& gid) {
        Group* group = getGroup(gid);
        if(!group) return;

        group->simplifyGroupDebts();
    }
};
Splitwise* Splitwise::instance = nullptr;



int main() {
    
    Splitwise* manager = Splitwise::getInstance();
    
    cout << endl << "=========== Creating Users ===================="<<endl;
    User* user1 = manager->createUser("Aditya", "aditya@gmail.com");
    User* user2 = manager->createUser("Rohit", "rohit@gmail.com");
    User* user3 = manager->createUser("Manish", "manish@gmail.com");
    User* user4 = manager->createUser("Saurav", "saurav@gmail.com");
    
    cout << endl << "=========== Creating Group and Adding Members ===================="<<endl;
    Group* hostelGroup = manager->createGroup("Hostel Expenses");
    manager->addUserToGroup(user1->userId, hostelGroup->groupId);
    manager->addUserToGroup(user2->userId, hostelGroup->groupId);
    manager->addUserToGroup(user3->userId, hostelGroup->groupId);
    manager->addUserToGroup(user4->userId, hostelGroup->groupId);

    cout << endl << "=========== Adding Expenses in group ===================="<<endl;    
    vector<string> groupMembers = {user1->userId, user2->userId, user3->userId, user4->userId};
    manager->addExpenseToGroup(hostelGroup->groupId, "Lunch", 800.0, user1->userId, groupMembers, SplitTypes::EQUAL);
    
    vector<string> dinnerMembers = {user1->userId, user3->userId, user4->userId};
    vector<double> dinnerAmounts = {200.0, 300.0, 200.0};
    manager->addExpenseToGroup(hostelGroup->groupId, "Dinner", 700.0, user3->userId, dinnerMembers, 
                             SplitTypes::EXACT, dinnerAmounts);

    cout << endl << "=========== printing Group-Specific Balances ===================="<<endl; 
    manager->showGroupBalances(hostelGroup->groupId);

    cout << endl << "=========== Debt Simplification ===================="<<endl; 
    manager->simplifyGroupDebts(hostelGroup->groupId);

    cout << endl << "=========== printing Group-Specific Balances ===================="<<endl; 
    manager->showGroupBalances(hostelGroup->groupId);

    cout << endl << "=========== Adding Individual Expense ===================="<<endl; 
    manager->addIndividualExpense("Coffee", 40.0, user2->userId, user4->userId, SplitTypes::EQUAL);
    
    cout << endl << "=========== printing User Balances ===================="<<endl; 
    manager->showUserBalance(user1->userId);
    manager->showUserBalance(user2->userId);
    manager->showUserBalance(user3->userId);
    manager->showUserBalance(user4->userId);

    cout << endl << "==========Attempting to remove Rohit from group==========" << endl;
    manager->removeUserFromGroup(user2->userId, hostelGroup->groupId);

    cout << endl << "======== Making Settlement to Clear Rohit's Debt =========="<<endl; 
    manager->settlePaymentInGroup(hostelGroup->groupId, user2->userId, user3->userId, 200.0);
    
    cout << endl << "======== Attempting to Remove Rohit Again =========="<<endl;
    manager->removeUserFromGroup(user2->userId, hostelGroup->groupId);
    
    cout << endl << "=========== Updated Group Balances ===================="<<endl; 
    manager->showGroupBalances(hostelGroup->groupId);
    
    return 0;
}