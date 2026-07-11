#include <iostream>
#include <mutex>
using namespace std;

class Singleton {
private:
    static Singleton* instance;

    Singleton() {
        cout << "Singleton constructor called" << endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        return instance;
    }
};

// init static members
Singleton* Singleton::instance = new Singleton(); // defined object before creating a actual object 

int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    cout << (s1 == s1) << endl;
}