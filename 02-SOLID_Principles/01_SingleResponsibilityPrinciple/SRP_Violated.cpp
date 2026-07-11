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
        void printInvoice(){
            cout << "Shopping Card Invoice: " << endl;
            for(auto p : products){
                cout << p->name << " - " << p->price << endl;
            }

            cout << "Total: " << calculateTotalPrice() << endl;
        }

        // must be in separate class
        void saveToDB(){
            cout << "Data saved to Database!" << endl;
            return;
        }
};

/*
NOTE: this code will work but it's breaking the Single Responsiblity Principle. If codebase gets bigger then it will create lots of issues during new feature
    integration.
*/

int main()
{
    ShoppingCart* cart = new ShoppingCart();
    cart->addProduct(new Product("Laptop", 6500.86));
    cart->addProduct(new Product("Mouse", 250.24));

    cart->printInvoice();
    cart->saveToDB();

    return 0;
}
