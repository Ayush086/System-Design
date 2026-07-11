#include <iostream>
#include <mutex>
using namespace std;


class Singleton {
private:
    static Singleton* instance;
    static mutex mtx;
    Singleton() {
        cout << "Singleton constructor called. New object created" << endl;
    }
    // prevent copy and assignment
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
public:
    static Singleton* getInstance() {
        if(instance==nullptr){
            lock_guard<mutex> loct(mtx); // to make it thread safe
            if(instance==nullptr) {
                instance = new Singleton();
            }
        }
        return instance;
    }
};

// initiate the static var
Singleton* Singleton::instance = nullptr;
mutex Singleton::mtx;

int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    cout << (s1 == s2) << endl;
}