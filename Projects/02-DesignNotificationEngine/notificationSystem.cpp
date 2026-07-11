#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// abstract notification class
class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() {} // virtual destructor
};

// Concrete notification class : text notification
class SimpleNotification : public INotification {
private:
    string text;
public:
    SimpleNotification(const string& msg){
        text = msg;
    }

    string getContent() const override {
        return text;
    }
};

                                        /* DECORATOR COMPONENTS */
// abstract decorator class
class INotificationDecorator : public INotification {
protected:
    INotification* notification;
public:
    INotificationDecorator(INotification* n){
        notification = n;
    }

    virtual ~INotificationDecorator() {
        delete notification;
    }
};

// concrete decorator class : timestamp decorator
class TimeStampDecorator : public INotificationDecorator {
public:
    TimeStampDecorator(INotification* n) : INotificationDecorator(n) {} // pointing to base class's notification member variable

    string getContent() const override {
        return "[2026-05-22 17:16:36] " + notification->getContent();
    }
};

// concrete decoartor class : signature decorator
class SignatureDecorator : public INotificationDecorator {
private:
    string signature;
public:
    SignatureDecorator(INotification* n, const string& sign) : INotificationDecorator(n) { // indirectly pointing to INotificationDecorator's member variable
        signature = sign;
    } 

    string getContent() const override {
        return notification->getContent() + "\n-- " + signature + "\n\n";
    }
};



                                        /*OBSERVER DESIGN COMPONENTS*/
// abstract observer class
class IObserver{
public:
    virtual void update() = 0;  
    virtual ~IObserver() {} // virtual destructor
};

// abstract observable class
class IObservable{
public:
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObserver() = 0;
};

// concrete Observable : Notification oberservable
class NotificationObservable : public IObservable {
protected:
    INotification* currentNotification;
    vector<IObserver*> observers;

public:
    // constructor
    NotificationObservable(){
        currentNotification = nullptr;
    }

    void addObserver(IObserver* obs) override {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) override {
        observers.erase(remove(observers.begin(), observers.end(), obs), observers.end()); // findout and delete
    }

    void notifyObserver() override {
        for(unsigned int i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }

    void setNotification(INotification* notification) {
        if(currentNotification != nullptr) delete currentNotification;

        // set and notify all
        currentNotification = notification;
        notifyObserver();
    }

    INotification* getNotification() {
        return currentNotification;
    }

    string getNotificationContent() {
        return currentNotification->getContent();
    }

    // destructor
    ~NotificationObservable() {
        if(currentNotification != NULL) {
            delete currentNotification;
        }
    }
};


// concrete observer : logging
class Logger : public IObserver {
private:
    NotificationObservable* notificationObservable;

public:
    // default constructor -- to avoid lot's of configuration at client side
    // Logger() {
    //     this->notificationObservable = NotificationService::getInstance()->getObservable();
    //     notificationObservable->addObserver(this);
    // }

    // Moved definition below NotificationService
    Logger();

    // constructor
    Logger(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void update() {
        cout << "Logging New Notification : " << notificationObservable->getNotificationContent() << endl;
    }
};



                                        /* NOTIFICATION ENGINE STRATEGY DESIGN */
// abstract strategy class
class INotificationStrategy {
public:
    virtual void sendNotification(string content) = 0;
};

// concrete strategy : email notification
class EmailNotificationStrategy : public INotificationStrategy{
private:
    string emailId;
public:
    EmailNotificationStrategy(string email){
        this->emailId = email;
    }

    void sendNotification(string content) override {
        // simulate email sending process
        cout << "Sent email notification to: " << emailId << " {\n" << content << "}" << endl;
    }
};

// concrete startegy : sms notification
class SMSNotificationStrategy : public INotificationStrategy {
private:
    string phoneNumber;
public:
    SMSNotificationStrategy(string phoneNo){
        this->phoneNumber = phoneNo;
    }

    void sendNotification(string content) override {
        cout << "Sent SMS notification on: " << phoneNumber << " {\n" << content << "}" << endl;
    }
};

// concrete startegy : push notification
class PushNotificationStrategy : public INotificationStrategy {
public:
    void sendNotification(string content) override {
        cout << "Sent Push notification: " << " {\n" << content << "}" << endl;
    }
};


// concrete observer : notification engine
class NotificationEngine : public IObserver {
private:
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy* > notificationStrategies;

public:
    // default constructor
    // NotificationEngine() {
    //     this->notificationObservable = NotificationService::getInstance()->getObservable();
    //     notificationObservable->addObserver(this);
    // }

    // Moved definition below NotificationService
    NotificationEngine();

    NotificationEngine(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void addNotificationStrategy(INotificationStrategy* ns) {
        this->notificationStrategies.push_back(ns);
    }

    void removeNotificationStrategy(INotificationStrategy* ns){
        this->notificationStrategies.erase(remove(notificationStrategies.begin(), notificationStrategies.end(), ns), notificationStrategies.end());
    }

    void update() {
        string notificationContent = notificationObservable->getNotificationContent();
        for(const auto notificationStrategy : notificationStrategies){
            notificationStrategy->sendNotification(notificationContent);
        }
    }
};


                                        /* NOTIFICATION SERVICE */
// client code will interact with this service
// singleton class
class NotificationService {
private:
    NotificationObservable* observable;
    static NotificationService* instance;
    vector<INotification*> notifications;

    NotificationService() {
        // private constructor
        observable = new NotificationObservable();
    }

public:
    static NotificationService* getInstance() {
        if(instance == nullptr){
            instance = new NotificationService();
        }

        return instance;
    }

    NotificationObservable* getObservable() {
        return observable;
    }

    void sendNotification(INotification* notification){
        notifications.push_back(notification); // storing the list of notification which are sent
        observable->setNotification(notification);
    }

    ~NotificationService(){
        delete observable;
    }
};
NotificationService* NotificationService::instance = nullptr; // invoked static variable





// New version: define default constructors after NotificationService is fully declared.
Logger::Logger() {
    this->notificationObservable = NotificationService::getInstance()->getObservable();
    notificationObservable->addObserver(this);
}

NotificationEngine::NotificationEngine() {
    this->notificationObservable = NotificationService::getInstance()->getObservable();
    notificationObservable->addObserver(this);
}

int main() {
    // create service
    NotificationService* notificationService = NotificationService::getInstance();

    // enable logging
    Logger* logger = new Logger(); // calling default constructor internally

    // create notification engine
    NotificationEngine* engine = new NotificationEngine(); // calling default constructor

    engine->addNotificationStrategy(new EmailNotificationStrategy("email@example.com"));
    engine->addNotificationStrategy(new SMSNotificationStrategy("85206644120"));
    engine->addNotificationStrategy(new PushNotificationStrategy());

    // add decorators to notification
    INotification* notification = new SimpleNotification("New Learning!");
    notification = new TimeStampDecorator(notification);
    notification = new SignatureDecorator(notification, "Ayush U.");

    notificationService->sendNotification(notification);

    delete logger;
    delete engine;


    return 0;
}

// int main() {
//     // create service
//     NotificationService* notificationService = NotificationService::getInstance();

//     // get observable
//     NotificationObservable* notificationObservable = notificationService->getObservable();

//     // enable logging
//     Logger* logger = new Logger(notificationObservable);

//     // create notification engine
//     NotificationEngine* engine = new NotificationEngine(notificationObservable);

//     engine->addNotificationStrategy(new EmailNotificationStrategy("email@example.com"));
//     engine->addNotificationStrategy(new SMSNotificationStrategy("85206644120"));
//     engine->addNotificationStrategy(new PushNotificationStrategy());

//     // integrate observers
//     notificationObservable->addObserver(logger);
//     notificationObservable->addObserver(engine);

//     // add decorators to notification
//     INotification* notification = new SimpleNotification("New Learning!");
//     notification = new TimeStampDecorator(notification);
//     notification = new SignatureDecorator(notification, "Ayush U.");

//     notificationService->sendNotification(notification);

//     delete logger;
//     delete engine;


//     return 0;
// }

