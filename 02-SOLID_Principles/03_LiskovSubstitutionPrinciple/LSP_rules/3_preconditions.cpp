#include <iostream>
using namespace std;

// precondition must be satisfied before a method can be executed

class User {
    public:
        virtual void setPassword(string pass) {
            if (pass.length() < 8) {
                throw invalid_argument("password must be atleat 8 chars long!");
            }
            cout << "Password set successfully" << endl;
        }
};

class AdminUser : public User {
    public:
        void setPassword(string pass) override {
            if (pass.length() < 6) {
                throw invalid_argument("Passwrod must be at leat 6 chars long");
            }
            cout << "Password set successfully" << endl;
        }
};

int main() {
    // User* user = new AdminUser();
    User* user = new User();
    user->setPassword("Admin1");  // this will work since AdminUser allows shorter length password

    return 0;
}