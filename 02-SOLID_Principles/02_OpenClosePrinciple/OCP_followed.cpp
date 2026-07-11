#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Item in a shopping cart
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

// abstract class
class StoragePersistance {
    private: 
        ShoppingCart* cart;

    public: 
        virtual void save(ShoppingCart* cart) = 0; // for function/method overridding

        
};

// derived classes
class MongoDBPersistance : public StoragePersistance {
    public:
        // function overridding
        void save(ShoppingCart* cart) override {
            cout << "Data saved to MongoDB Successfully." << endl;
        }
};

class SQLPersistance : public StoragePersistance {
    public:
        // function overridding
        void save(ShoppingCart* cart) override {
            cout << "Data saved to SQL DB Successfully." << endl;
        }
};

/*
NOTE: This above class's code is not breaking open-close principle logic. We have created a abstract class and which is now used for different purpose by performing method overridding.
*/

int main() {
    ShoppingCart* cart = new ShoppingCart();
    cart->addProduct(new Product("Laptop", 6500.86));
    cart->addProduct(new Product("Mouse", 250.24));

    Invoice* invoice = new Invoice(cart);
    invoice->printInvoice();

    StoragePersistance* mongo = new MongoDBPersistance();
    StoragePersistance* sql = new SQLPersistance();

    mongo->save(cart);
    sql->save(cart);

    return 0;
}
