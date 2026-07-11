// METHOD ARGUMENT RULE
#include <iostream>
using namespace std;


class Parent{
public:
    virtual void solve(string s){
        cout << "parent solve method " << s << endl;
    }
};

class Child : public Parent {
public:
    void solve(string msg) override {
        cout << "Child solve " << msg << endl;
    }
};

// Client that pass string as msg as client expects
class Client {
    private:
        Parent* p;

    public:
        Client(Parent* p){
            this->p = p;
        }
        void printMsg() {
            p->solve("New Learning");
        }
};

int main() {
    Parent* parent = new Parent();
    Parent* child = new Child();

    Client* client = new Client(parent);

    client->printMsg();

    return 0;
}