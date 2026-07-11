/* Payment Gateway System                                                                                                                       Date: 21/06/2026 */
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// structure of payment request
struct PaymentRequest {
    string sender;
    string receiver;
    double amount;
    string currency;

    PaymentRequest(const string& sender, const string& receiver, double amt, const string& curr) {
        this->sender = sender;
        this->receiver = receiver;
        this->amount = amt;
        this->currency = curr;
    }
};


// banking system abstract class (banks internal execution of a transaction) - not a part or our project
class IBankingSystem {
public: 
    virtual bool processPayment(double amount) = 0;
    virtual ~IBankingSystem() {}
};

class PaytmBankingSystem : public IBankingSystem {
public:
    PaytmBankingSystem() {}

    bool processPayment(double amt) override {
        cout << "[BankingSystem-PayTM] processing payment of " << amt << "..." << endl;
        // to simulate 20% success rate
        int r = rand() % 100;
        return r < 20;
    }

};

class RazorpayBankingSystem : public IBankingSystem {
public:
    RazorpayBankingSystem() {}

    bool processPayment(double amt) override {
        cout << "[BankingSystem-Razorpay] processing payment of " << amt << "..." << endl;
        // to simulate 90% success rate
        int r = rand() % 100;
        return r < 90;
    }

};


// abstract class - template design pattern
class PaymentGateway {
protected:
    IBankingSystem* bankingSystem;

public: 
    PaymentGateway() {
        bankingSystem = nullptr;
    }

    virtual ~PaymentGateway() {
        delete bankingSystem;
    }

    // payment flow template
    virtual bool processPayment(PaymentRequest* req) {
        if(!validatePayment(req)){
            cout << "[PaymentGateway] validation failed for " << req->sender << endl;
            return false;
        }
        if(!initiatePayment(req)) {
            cout << "[PaymentGateway] initiation failed for " << req->sender << endl;
            return false;
        }
        if(!confirmPayment(req)) {
            cout << "[PaymentGateway] confirmation failed for " << req->sender << endl;
            return false;
        }

        return true;
    }

    // this methods depends upon the gateway types
    virtual bool validatePayment(PaymentRequest* req) = 0;
    virtual bool initiatePayment(PaymentRequest* req) = 0;
    virtual bool confirmPayment(PaymentRequest* req) = 0;
};


// concrete payment gateway for PayTM
class PaytmGateway : public PaymentGateway {
public:
    PaytmGateway() {
        bankingSystem = new PaytmBankingSystem();
    }

    bool validatePayment(PaymentRequest* req) override {
        cout << "[Paytm] validating payment for " << req->sender << endl;

        if(req->amount <= 0 || req->currency != "INR") return false;

        return true;
    }

    bool initiatePayment(PaymentRequest* req) override {
        cout << "[Paytm] initiating payment of " << req->amount << " " << req->currency << " for " << req->sender << endl;
        return bankingSystem->processPayment(req->amount);
    }

    bool confirmPayment(PaymentRequest* req) override {
        cout << "[Paytm] confirming payment for " << req->sender << endl;
        return true;
    }
};

// concrete razorpay gateway for PayTM
class RazorpayGateway : public PaymentGateway {
public:
    RazorpayGateway() {
        bankingSystem = new RazorpayBankingSystem();
    }

    bool validatePayment(PaymentRequest* req) override {
        cout << "[Razorpay] validating payment for " << req->sender << endl;

        if(req->amount <= 0 || req->currency != "INR") return false;

        return true;
    }

    bool initiatePayment(PaymentRequest* req) override {
        cout << "[Razorpay] initiating payment of " << req->amount << " " << req->currency << " for " << req->sender << endl;
        return bankingSystem->processPayment(req->amount);
    }

    bool confirmPayment(PaymentRequest* req) override {
        cout << "[Razorpay] confirming payment for " << req->sender << endl;
        return true;
    }
};


// proxy class that wraps gateways to add retry mechanism (proxy pattern)
class PaymentGatewayProxy : public PaymentGateway {
private:
    PaymentGateway* realGateway;
    int retries;

public:
    PaymentGatewayProxy(PaymentGateway* gateway, int maxRetries) {
        this->realGateway = gateway;
        this->retries = maxRetries;
    }

    ~PaymentGatewayProxy() {
        delete realGateway;
    }

    bool processPayment(PaymentRequest* request) override {
        bool result = false;
        for(int attempt = 0; attempt < retries; ++attempt) {
            if(attempt > 0) {
                cout << "[Proxy] retrying payment (attempt) " << (attempt + 1) << ") for " << request->sender  << endl;
            }

            result = realGateway->processPayment(request);
            if(result) break;
        }

        if(!result) cout << "[Proxy] Payment failed after " << retries << " attmepts for " << request->sender << endl;

        return result;
    }

    bool validatePayment(PaymentRequest* req) override {
        return realGateway->validatePayment(req);
    }

    bool initiatePayment(PaymentRequest* req) override {
        return realGateway->initiatePayment(req);
    }

    bool confirmPayment(PaymentRequest* req) override {
        return realGateway->confirmPayment(req);
    }
};



// gateway factory to create single gateway
enum class GatewayType {
    PAYTM, RAZORPAY
};

class GatewayFactory {
private:
    static GatewayFactory instance;
    GatewayFactory() {}
    // delete copy assignement to ensure it's singleton
    GatewayFactory(const GatewayFactory&) = delete;
    GatewayFactory& operator=(const GatewayFactory&) = delete;

public:
    static GatewayFactory& getInstance() {
        return instance;
    }

    PaymentGateway* getGateway(GatewayType type) {
        if(type == GatewayType::PAYTM) {
            PaymentGateway* paymentGateway = new PaytmGateway();
            return new PaymentGatewayProxy(paymentGateway, 5);
        } else { // razorpay gateway
            PaymentGateway* paymentGateway = new RazorpayGateway();
            return new PaymentGatewayProxy(paymentGateway, 3);
        }
    }
};

// define instance of singleton class
GatewayFactory GatewayFactory::instance;


// API service layer (singleton)
class PaymentService {
private:
    static PaymentService instance;
    PaymentGateway* gateway;

    PaymentService() {
        gateway = nullptr;
    }

    ~PaymentService() {
        delete gateway;
    }

    PaymentService(const PaymentService&) = delete;
    PaymentService& operator=(const PaymentService&) = delete;

public:
    static PaymentService& getInstance() {
        return instance;
    }

    void setGateway(PaymentGateway* g) {
        if(gateway) delete gateway;
        gateway = g;
    }

    bool processPayment(PaymentRequest* request) {
        if(!gateway) {
             cout << "[PaymentService] No payment gateway selected" << endl;
             return false;
        }
        return gateway->processPayment(request);
    }
};
PaymentService PaymentService::instance;


// controller to receive client requests (singleton)
class PaymentController {
private:
    static PaymentController instance;
    PaymentController() {}
    PaymentController(const PaymentController&) = delete;
    PaymentController& operator=(const PaymentController&) = delete;

public: 
    static PaymentController& getInstance() {
        return instance;
    }

    bool handlePayment(GatewayType type, PaymentRequest* req) {
        PaymentGateway* paymentGateway = GatewayFactory::getInstance().getGateway(type);
        PaymentService::getInstance().setGateway(paymentGateway);
        return PaymentService::getInstance().processPayment(req);
    }
};
PaymentController PaymentController::instance;



// client
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    PaymentRequest* req1 = new PaymentRequest("Rahul", "Virat", 100.69, "INR");
    
    cout << "**Processing payment via Paytm**" << endl;
    cout << "-------------------------------" << endl;
    bool res1 = PaymentController::getInstance().handlePayment(GatewayType::PAYTM, req1);
    cout << "Result: " << (res1 ? "SUCCESS" : "FAIL") << endl;
    cout << "-------------------------------" << endl;
    
    PaymentRequest* req2 = new PaymentRequest("Rohit", "Nitish", 1001.69, "INR");

    cout << endl << "**Processing payment via Razorpay**" << endl;
    cout << "-------------------------------" << endl;
    bool res2 = PaymentController::getInstance().handlePayment(GatewayType::RAZORPAY, req2);
    cout << "Result: " << (res2 ? "SUCCESS" : "FAIL") << endl;
    cout << "-------------------------------" << endl;

    return 0;
}