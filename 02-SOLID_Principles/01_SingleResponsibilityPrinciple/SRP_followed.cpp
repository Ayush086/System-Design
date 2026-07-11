#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Product {
    public:
        string name;
        double price;

    // constructor
    Product(string name, double price){
        this->name = name;
        this->price = price;
    }
};

// This class will only responsible for cart related logic
class ShoppingCart{
    private:
        vector<Product*> products; // one to many relation
    
    public:
        void addProduct(Product *p){
            products.push_back(p);
        }

        const vector<Product*> getProducts() {
            return products;
        }

        double calculateTotalPrice(){
            double totalPrice = 0;
            for(auto p: products){
                totalPrice += p->price;
            }

            return totalPrice;
        }

        // must be in separate class
        // void printInvoice(){
        //     cout << "Shopping Card Invoice: " << endl;
        //     for(auto p : products){
        //         cout << p->name << " - " << p->price << endl;
        //     }

        //     cout << "Total: " << calculateTotalPrice() << endl;
        // }

        // must be in separate class
        // void saveToDB(){
        //     cout << "Data saved to Database!" << endl;
        //     return;
        // }
};

class Invoice {
    private: 
        ShoppingCart* cart;

    public:
        Invoice(ShoppingCart* cart) {
            this->cart = cart;
        }

        void printInvoice() {
            cout << "Shopping Cart Invoice: " << endl;
            for(auto p: cart->getProducts()){
                cout << p->name << " - " << p->price << endl;
            }

            cout << "Total: " << cart->calculateTotalPrice() << endl;
        }
};


class Storage {
    private: 
        ShoppingCart* cart;

    public: 
        Storage(ShoppingCart* cart){
            this->cart = cart;
        }

        void saveToDB() {
            cout << "Data saved to Database successfully!" << endl;
        }
};

/*
NOTE: this code will work and it's following Single Responsiblity Principle. If codebase gets bigger then it will not create any issues during new feature
    integration. Because every class has their own responsibility now
*/

int main()
{
    ShoppingCart* cart = new ShoppingCart();
    cart->addProduct(new Product("Laptop", 6500.86));
    cart->addProduct(new Product("Mouse", 250.24));

    Invoice* invoice = new Invoice(cart);
    invoice->printInvoice();

    Storage* store = new Storage(cart);
    store->saveToDB();

    return 0;
}
