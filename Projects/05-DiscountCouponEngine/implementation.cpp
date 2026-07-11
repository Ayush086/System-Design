/* Discount Coupon Engine                                                                                                                       Date: 27/06/2026 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

class PlatformMutex {
private:
    CRITICAL_SECTION cs;

public:
    PlatformMutex() {
        InitializeCriticalSection(&cs);
    }

    ~PlatformMutex() {
        DeleteCriticalSection(&cs);
    }

    PlatformMutex(const PlatformMutex&) = delete;
    PlatformMutex& operator=(const PlatformMutex&) = delete;

    void lock() {
        EnterCriticalSection(&cs);
    }

    void unlock() {
        LeaveCriticalSection(&cs);
    }
};

class LockGuard {
private:
    PlatformMutex& mtx;

public:
    LockGuard(PlatformMutex& mutex) : mtx(mutex) {
        mtx.lock();
    }

    ~LockGuard() {
        mtx.unlock();
    }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

enum StrategyType {
    FLAT, PERCENT, PERCENT_WITH_CAP
};

// Discount Strategy (Strategy Pattern)
class DiscountStrategy {
public:
    virtual ~DiscountStrategy() {}
    virtual double calculate(double baseAmount) = 0;
};

class FlatDiscountStrategy : public DiscountStrategy {
private:
    double amount;
public:
    FlatDiscountStrategy(double amt){
        this->amount = amt;
    }

    double calculate(double baseAmount) override {
        return min(amount, baseAmount);
    }
};

class PercentageDiscountStrategy : public DiscountStrategy {
private:
    double percent;
public:
    PercentageDiscountStrategy(double p){
        this->percent = p;
    }

    double calculate(double baseAmount) override{
        return (this->percent/100.0) * baseAmount;
    }
};

class PercentageWithCapDiscountStrategy : public DiscountStrategy {
private:
    double percent;
    double cap;
public:
    PercentageWithCapDiscountStrategy(double p, double c) {
        this->percent = p;
        this->cap = c;
    }

    double calculate(double baseAmount) override {
        double disc = (percent / 100.0) * baseAmount;
        if(disc > cap) return cap;
        return disc;
    }
};


// discount strategy manager (singleton)
class DiscountStrategyManager {
private:    
    DiscountStrategyManager() {}
    DiscountStrategyManager(const DiscountStrategyManager&) = delete;
    DiscountStrategyManager& operator=(const DiscountStrategyManager&) = delete;

public:
    static DiscountStrategyManager* getInstance() {
        static DiscountStrategyManager instance;
        return &instance;
    }

    DiscountStrategy* getStrategy(StrategyType type, double param1, double param2 = 0.0) const {
        if(type == StrategyType::FLAT){
            return new FlatDiscountStrategy(param1);
        }
        if(type == StrategyType::PERCENT) {
            return new PercentageDiscountStrategy(param1);
        }
        if(type == StrategyType::PERCENT_WITH_CAP) {
            return new PercentageWithCapDiscountStrategy(param1, param2);
        }

        return nullptr;
    }
};


// assumed existing cart and product class
class Product {
private:    
    string name;
    string category;
    double price;
public: 
    Product(string name, string category, double price){
        this->name = name;
        this->category = category;
        this->price = price;
    }

    string getName() {
        return this->name;
    }

    string getCategory() const {
        return this->category;
    }

    double getPrice() {
        return this->price;
    }
};

class CartItem {
private:    
    Product* product;
    int qty;
public: 
    CartItem(Product* prod, int qty) {
        this->product = prod;
        this->qty = qty;
    }

    double itemTotal() {
        return this->product->getPrice() * qty;
    }

    const Product* getProduct() {
        return this->product;
    }
};

class Cart {
private:    
    vector<CartItem*> items;
    double originalTotal;
    double currentTotal;
    bool isLoyal;
    string paymentBank;

public: 
    Cart() {
        this->originalTotal = 0.0;
        this->currentTotal = 0.0;
        this->isLoyal = false;
        this->paymentBank = "";
    }

    void addProduct(Product* prod, int qty = 1) {
        CartItem* item = new CartItem(prod, qty);
        items.push_back(item);
        originalTotal += item->itemTotal();
        currentTotal += item->itemTotal();
    }

    double getOriginalTotal() {
        return this->originalTotal;
    }

    double getCurrentTotal() {
        return this->currentTotal;
    }

    void applyDiscount(double d) {
        currentTotal -= d;
        if(currentTotal < 0) currentTotal = 0;
    }

    void setLoyalMember(bool member){
        this->isLoyal = member;
    }

    bool isLoyalMember() {
        return this->isLoyal;
    }

    void setPaymentBank(string bank) {
        this->paymentBank = bank;
    }

    string getPaymentBank() {
        return this->paymentBank;
    }

    vector<CartItem*> getItems() {
        return this->items;
    }
};



// Coupon (Chain of Responsibility)
class Coupon {
private:
    Coupon* next;
public:
    Coupon() {
        this->next = nullptr;
    }

    virtual ~Coupon() {
        if(next){
            delete next;
        }
    }

    void setNext(Coupon* nxt) {
        this->next = nxt;
    }

    Coupon* getNext() {
        return next;
    }

    virtual bool isApplicable(Cart* cart) = 0;
    virtual double getDiscount(Cart* cart) = 0;
    virtual bool isCombinable() {
        return true;
    }
    virtual string name() = 0;

    void applyDiscount(Cart* cart) {
        if(isApplicable(cart)){
            double discount = getDiscount(cart);
            cart->applyDiscount(discount);
            cout << name() << " applied: " << discount << endl;
            if(!isCombinable()) {
                return;
            }
        }
        if(next) next->applyDiscount(cart);
    }
};

// concrete coupon classes
class SeasonalOffer : public Coupon {
private:
    double percent;
    string category;
    DiscountStrategy* strat;
public:
    SeasonalOffer(double pct, string cat) {
        this->percent = pct;
        this->category = cat;
        this->strat = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT, percent);
    }

    ~SeasonalOffer(){
        delete strat;
    }

    bool isApplicable(Cart* cart) override {
        for(CartItem* item : cart->getItems()) {
            if(item->getProduct()->getCategory() == category) return true;
        }
        return false;
    }

    double getDiscount(Cart* cart) override {
        double subtotal = 0.0;
        for(CartItem* item: cart->getItems()) {
            if(item->getProduct()->getCategory() == category){
                subtotal += item->itemTotal();
            }
        }
        return strat->calculate(subtotal);
    }

    bool isCombinable() override {
        return true;
    }

    string name() override {
        return "Seasonal Offer " + to_string((int)percent) + "% off " + category;
    }
};

class LoyaltyDiscount : public Coupon {
private:
    double percent;
    DiscountStrategy* strat;
public:
    LoyaltyDiscount(double pct) {
        this->percent = pct;
        this->strat = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT, percent);
    }

    ~LoyaltyDiscount() {
        delete strat;
    }

    bool isApplicable(Cart* cart) override {
        return cart->isLoyalMember();
    }

    double getDiscount(Cart* cart) override {
        return strat->calculate(cart->getCurrentTotal());
    }

    string name() override {
        return "Loyalty Discount " + to_string((int)this->percent) + "% off";
    }
};

class BulkPurchaseDiscount : public Coupon {
private:
    double threshold;
    double flatOff;
    DiscountStrategy* strat;

public:
    BulkPurchaseDiscount(double thr, double off) {
        this->threshold = thr;
        this->flatOff = off;
        this->strat = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::FLAT, flatOff);
    }

    ~BulkPurchaseDiscount() {
        delete strat;
    }

    bool isApplicable(Cart* cart) override {
        return cart->getOriginalTotal() >= this->threshold;
    }

    double getDiscount(Cart* cart) override {
        return strat->calculate(cart->getCurrentTotal());
    }

    string name() override {
        return "Bulk Purchase Rs. " + to_string((int)flatOff) + " off over" + to_string((int)threshold);
    }
};

class BankingCoupon : public Coupon {
private:
    string bank;
    double minSpend;
    double percent;
    double offCap;
    DiscountStrategy* strat;

public:
    BankingCoupon(const string& bank, double ms, double pct, double offCap) {
        this->bank = bank;
        this->minSpend = ms;
        this->percent = pct;
        this->offCap = offCap;
        this->strat = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT_WITH_CAP, percent, offCap);
    }

    ~BankingCoupon() {
        delete strat;
    }

    bool isApplicable(Cart* cart) override {
        return cart->getPaymentBank() == bank && cart->getOriginalTotal() >= this->minSpend;
    }

    double getDiscount(Cart* cart) override {
        return this->strat->calculate(cart->getCurrentTotal());
    }

    string name() override {
        return bank + " Bank Rs " + to_string((int)percent) + " off upto " + to_string((int)offCap);
    }
};


// coupon manager (singleton)
class CouponManager{
private:
    Coupon* head;
    mutable PlatformMutex mtx;
    CouponManager() {
        head = nullptr;
    }

    CouponManager(const CouponManager&) = delete;
    CouponManager& operator=(const CouponManager&) = delete;

public:
    static CouponManager* getInstance() {
        static CouponManager instance;
        return &instance;
    }

    void registerCoupon(Coupon* coupon) {
        LockGuard lock(mtx);
        if(!head) head = coupon;
        else {
            Coupon* cur = head;
            while(cur->getNext()) {
                cur = cur->getNext();
            }
            cur->setNext(coupon);
        }
    }

    vector<string> getApplicable(Cart* cart) const {
        LockGuard lock(mtx);
        vector<string> res;
        Coupon* cur = head;
        while(cur) {
            if(cur->isApplicable(cart)){
                res.push_back(cur->name());
            }
            cur = cur->getNext();
        }
        return res;
    }

    double applyAll(Cart* cart) {
        LockGuard lock(mtx);
        if(head) {
            head->applyDiscount(cart);
        }
        return cart->getCurrentTotal();
    }
};


// client
int main() {

    
    Product* p1 = new Product("Winter Jacket", "Clothing", 1000);
    Product* p2 = new Product("Smartphone", "Electronics", 20000);
    Product* p3 = new Product("Jeans", "Clothing", 1000);
    Product* p4 = new Product("Headphones", "Electronics", 2000);
    
    Cart* cart = new Cart();
    cart->addProduct(p1, 1);
    cart->addProduct(p2, 1);
    cart->addProduct(p3, 2);
    cart->addProduct(p4, 1);
    cart->setLoyalMember(true);
    cart->setPaymentBank("ABC");
    
    cout << "Original Cart Total: " << cart->getOriginalTotal() << " Rs" << endl;
    
    CouponManager* mgr = CouponManager::getInstance();
    mgr->registerCoupon(new SeasonalOffer(10, "Clothing"));
    mgr->registerCoupon(new LoyaltyDiscount(5));
    mgr->registerCoupon(new BulkPurchaseDiscount(1000, 100));
    mgr->registerCoupon(new BankingCoupon("ABC", 2000, 15, 500));
    

    vector<string> applicable = mgr->getApplicable(cart);
    cout << "Applicable Coupons:" << endl;
    for (string name : applicable) {
        cout << " - " << name << endl;
    }

    double finalTotal = mgr->applyAll(cart);
    cout << "Final Cart Total after discounts: " << finalTotal << " Rs" << endl;

    // Cleanup code
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete cart;

    return 0;
}