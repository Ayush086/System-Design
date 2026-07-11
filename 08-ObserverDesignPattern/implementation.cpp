#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// abstract observable class
class IChannel {
public:
    virtual void subscribe(ISubscriber* subsriber) = 0; // add()
    virtual void unsubscribe(ISubscriber* subscriber) = 0; // remove()
    virtual void notifySubscribers() = 0; // notify()
    virtual ~IChannel() {} // virtual destructor
};

// abstract observer class
class ISubscriber {
public:
    virtual void update() = 0;
    virtual ~ISubscriber() {}; // destructor
};


// concrete observable class
class Channel : public IChannel {
private:
    vector<ISubscriber*> subscribers;
    string name;
    string latestVideo;
public:
    Channel(const string& name){
        this->name = name;
    }

    // add a subscriber
    void subscribe(ISubscriber* subscriber) override {
        if(find(subscribers.begin(), subscribers.end(), subscriber) == subscribers.end()) {
            subscribers.push_back(subscriber);
        }
    }

    // remove a subscriber
    void unsubscribe(ISubscriber* subscriber) override {
        auto it = find(subscribers.begin(), subscribers.end(), subscriber);
        if(it != subscribers.end()) {
            subscribers.erase(it);
        }
    }

    // notify all subscribers of latest video
    void notifySubscribers() override {
        for(ISubscriber* sub : subscribers){
            sub->update();
        }
    }

    // upload video
    void uploadVideo(const string& title) {
        latestVideo = title;
        cout << "\n[" << name << " uploaded \"" << title << "\"]" << endl;
        notifySubscribers();
    }

    // read video data
    string getVideoData() {
        return "\nCheckout our new Video: " + latestVideo + "\n";
    }
};


// concrete observer class
class Subscriber : public ISubscriber {
private:
    string name;
    Channel* channel;

public:
    Subscriber(const string& name, Channel* channel){
        this->name = name;
        this->channel = channel;
    }

    void update() override {
        cout << "Hey there " << name << "," << this->channel->getVideoData();
    }
};

int main() {
    Channel* channel = new Channel("CodeHelp");

    Subscriber* sub1 = new Subscriber("Lakshya", channel);
    Subscriber* sub2 = new Subscriber("Ayush", channel);

    // subscribes to the channel
    channel->subscribe(sub1);
    channel->subscribe(sub2);

    // uploads a video
    channel->uploadVideo("Observer Design Pattern");

    // sub1 unsubscribes
    channel->unsubscribe(sub1);

    // uploads another video
    channel->uploadVideo("Object Oriented Programming in One Shot");

    return 0;
}