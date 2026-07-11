/* Building Zepto                                                                                                                 Date: 05/07/2026 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;

// product and it's factory
class Product{
private:
    int sku;
    string name;
    double price;

public: 
    Product(int id, string n, double p){
        this->sku = id;
        this->name = n;
        this->price = p;
    }

    int getSku() {
        return this->sku;
    }

    string getProductName() {
        return this->name;
    }

    double getPrice() {
        return this->price;
    }
};

class ProductFactory {
public:
    static Product* createProduct(int sku) {
        // actually it's supposed to be a DB call, but I'm trying to simulate it here since we don't have DB
        string name;
        double price;

        if(sku == 101){
            name = "Apple";
            price = 20;
        } else if (sku == 102) {
            name = "Banana";
            price = 10;
        } else if (sku == 103) {
            name = "Chocolate";
            price = 50;
        } else if (sku == 201) {
            name = "T-shirt";
            price = 500;
        } else if (sku == 202) {
            name  = "Jeans";
            price = 1000;
        } else {
            name  = "Item" + to_string(sku);
            price = 100;
        }
        return new Product(sku, name, price);
    }
};


// abstract class : inventory store 
class InventoryStore {
public: 
    virtual ~InventoryStore() {}
    virtual void addProduct(Product* prod, int qty) = 0;
    virtual void removeProduct(int sku, int qty) = 0;
    virtual int checkStock(int sku) = 0;
    virtual vector<Product*> listAvailableProducts() = 0;
};

// concrete class : DB Store
class DBInventoryStore : public InventoryStore {
private:    
    map<int, int>* stock;           // sku --> qty
    map<int, Product*>* products;   // sku --> product*

public:
    DBInventoryStore() {
        stock = new map<int, int>();
        products = new map<int, Product*>();
    }

    ~DBInventoryStore() {
        for(auto it: *products) {
            delete it.second;
        }

        delete products;
        delete stock;
    }

    void addProduct(Product* prod, int qty) override {
        int sku = prod->getSku();
        if(products->count(sku) == 0) {
            // doesn't exist
            (*products) [sku] = prod;
        } else {
            delete prod;
        }
        (*stock) [sku] += qty;
    }

    void removeProduct(int sku, int qty) override {
        if(stock->count(sku) == 0) return;

        int currentQuantity = (*stock) [sku];
        int remainQuantity = currentQuantity - qty;
        if(remainQuantity > 0) {
            (*stock)[sku] = remainQuantity;
        } else {
            stock->erase(sku);
        }
    }

    int checkStock(int sku) override {
        if(stock->count(sku) == 0) return 0;

        return (*stock)[sku];
    }

    vector<Product*> listAvailableProducts() override {
        vector<Product*> available;
        for (auto it : *stock) {
            int sku = it.first;
            int qty = it.second;
            if(qty > 0 && products->count(sku)) {
                available.push_back((*products)[sku]);
            }
        }
        return available;
    }
};


// singleton : inventory manager
class InventoryManager {
private:    
    InventoryStore* store;

public: 
    InventoryManager(InventoryStore* store) {
        this->store = store;
    }

    void addStock(int sku, int qty) {
        Product* prod = ProductFactory::createProduct(sku);
        store->addProduct(prod, qty);
        cout << "[InventoryManager] Added SKU " << sku << " Qty: " << qty << endl;
    }
    
    void removeStock(int sku, int qty) {
        store->removeProduct(sku, qty);
        cout << "[InventoryManager] Removed SKU " << sku << " Qty: " << qty << endl;
    }

    int checkStock(int sku) {
        return store->checkStock(sku);
    }

    vector<Product*> getaAvailableProducts() {
        return store->listAvailableProducts();
    }
};


// replenish strategy (strategy pattern)
class ReplenishStrategy {
public:
    virtual void replenish(InventoryManager* manager, map<int, int> itemsToReplenish) = 0;
    virtual ~ReplenishStrategy() {}
};

class ThresholdReplenishStrategy : public ReplenishStrategy {
private:    
    int threshold;

public:
    ThresholdReplenishStrategy(int threshold) {
        this->threshold = threshold;
    }

    void replenish(InventoryManager* manager, map<int, int> itemsToReplenish) override {
        cout << "[ThresholdReplenish] Checking threshold..." << endl;
        for(auto it : itemsToReplenish) {
            int sku = it.first;
            int qtyToAdd = it.second;
            int current = manager->checkStock(sku);
            if(current < threshold) {
                manager->addStock(sku, qtyToAdd);
                cout << "  -> SKU: " << sku << " was " << current <<", replenished by " << qtyToAdd << endl;
            }
        }
    }
};

class WeeklyReplenishStrategy : public ReplenishStrategy {
public:
    WeeklyReplenishStrategy() {}
    void replenish(InventoryManager* manager, map<int,int> itemsToReplenish) override {
        cout << "[WeeklyReplenish] Weekly replenishment triggered for inventory." << endl;
    }
};


class DarkStore {
private:
    string name;
    double x, y; // location
    InventoryManager* inventoryManager;
    ReplenishStrategy* replenishStrategy;

public:
    DarkStore(string n, double x_coord, double y_coord) {
        this->name = n;
        this->x = x_coord;
        this->y = y_coord;

        inventoryManager = new InventoryManager(new DBInventoryStore);
    }

    ~DarkStore() {
        delete inventoryManager;
        if(replenishStrategy) delete replenishStrategy;
    }

    double distanceTo(double ux, double uy) {
        return sqrt((x-ux)*(x-ux) + (y-uy)*(y-uy));
    }

    void runReplenishment(map<int, int> itemsToReplenish) {
        if(replenishStrategy) replenishStrategy->replenish(inventoryManager, itemsToReplenish);
    }

    // delegation methods
    vector<Product*> getAllProducts() {
        return inventoryManager->getaAvailableProducts();
    }

    int checkStock(int sku) {
        return inventoryManager->checkStock(sku);
    }

    void removeStock(int sku, int qty) {
        inventoryManager->removeStock(sku, qty);
    }

    void addStock(int sku, int qty) {
        Product* prod = ProductFactory::createProduct(sku);
        inventoryManager->addStock(sku, qty);
    }

    // getters + setters
    void setReplenishStrategy(ReplenishStrategy* strategy) {
        this->replenishStrategy = strategy;
    }

    string getName() {
        return this->name;
    }

    double getXCoordinate() {
        return this->x;
    }

    double getYCoordinate() {
        return this->y;
    }

    InventoryManager* getInventoryManager() {
        return this->inventoryManager;
    }
};


// dark store manager (singleton)
class DarkStoreManager {
private:
    vector<DarkStore*>* darkStores;
    static DarkStoreManager* instance;

    DarkStoreManager() {
        darkStores = new vector<DarkStore*>();
    }

public:
    static DarkStoreManager* getInstance() {
        if(instance == nullptr) {
            instance = new DarkStoreManager();
        }

        return instance;
    }

    void registerDarkStore(DarkStore* ds) {
        darkStores->push_back(ds);
    }

    vector<DarkStore*> getNearbyDarkStores(double ux, double uy, double maxDistance) {
        vector<pair<double, DarkStore*>> distList;
        for(auto ds : *darkStores) {
            double d = ds->distanceTo(ux, uy);
            if(d <= maxDistance) {
                distList.push_back(make_pair(d, ds));
            }
       }

       sort(distList.begin(), distList.end(), [](auto &a, auto &b){ return a.first < b.first;});

       vector<DarkStore*> result;
       for(auto &p: distList) {
        result.push_back(p.second);
       }

       return result;
    }

    ~DarkStoreManager() {
        for(auto ds : *darkStores){
            delete ds;
        }
        delete darkStores;
    }
};
DarkStoreManager* DarkStoreManager::instance = nullptr;


class Cart {
public:
    vector<pair<Product*,int>> items;  // (Product*, qty)

    void addItem(int sku, int qty) {
        Product* prod = ProductFactory::createProduct(sku);
        items.push_back(make_pair(prod, qty));
        cout << "[Cart] Added SKU " << sku << " (" << prod->getProductName() 
             << ") x" << qty << endl;
    }

    double getTotal() {
        double sum = 0.0;
        for (auto &it : items) {
            sum += (it.first->getPrice() * it.second);
        }
        return sum;
    }

    vector<pair<Product*,int>> getItems() {
        return items;
    }

    ~Cart() {
        for (auto &it : items) {
            delete it.first;
        }
    }
};

class User {
public:
    string name;
    double x, y;
    Cart* cart;  // User owns a cart

    User(string n, double x_coord, double y_coord) {
        name = n;
        x = x_coord;
        y = y_coord;
        cart = new Cart();
    }
    ~User() {
        delete cart;
    }

    Cart* getCart() {
        return cart;
    }
};

class DeliveryPartner {
public:
    string name;
    DeliveryPartner(string n) {
        this->name = n;
    }
};


// order + it's manager
class Order{
public:
    static int nextId;
    int orderId;
    User* user;
    vector<pair<Product*, int>> items;
    vector<DeliveryPartner*> partners;
    double totalAmount;

    Order(User* u) {
        orderId = nextId++;
        user = u;
        totalAmount = 0.0;
    }
};
int Order::nextId = 1;

class OrderManager {
private:
    vector<Order*>* orders;
    static OrderManager* instance;

    OrderManager() {
        this->orders = new vector<Order*> ();
    }

public:
    static OrderManager* getInstance() {
        if(instance == nullptr) {
            instance = new OrderManager();
        }

        return instance;
    }

    void placeOrder(User* user, Cart* cart) {
        cout << endl << "[OrderManager] Placeing order for " << user->name << endl;

        vector<pair<Product*, int>> requestedItems = cart->getItems();

        // find nearby dark stores
        double maxDist = 5.0;
        vector<DarkStore*> nearbyDarkStores =  DarkStoreManager::getInstance()->getNearbyDarkStores(user->x, user->y, maxDist);

        if(nearbyDarkStores.empty()) {
            cout << "   No dark stores within 5 KM. Cannot fulfill order. " << endl;
            return;
        }

        // check if closed store has all itmes
        DarkStore* firstStore = nearbyDarkStores.front();

        bool allInFirst = true;
        for(pair<Product*, int>& item: requestedItems) {
            int sku = item.first->getSku();
            int qty = item.second;

            if(firstStore->checkStock(sku) < qty) {
                allInFirst = false;
                break;
            }
        }

        Order* order = new Order(user); // order created

        if(allInFirst) {
            cout << "   All items at: " << firstStore->getName() << endl;

            for(pair<Product*, int>& item : requestedItems) {
                int sku = item.first->getSku();
                int qty = item.second;
                firstStore->removeStock(sku, qty);
                order->items.push_back({item.first, qty});
            }

            order->totalAmount = cart->getTotal();
            order->partners.push_back(new DeliveryPartner("partner1"));
            cout << "   Assigned Delivery Partner: partner1" << endl;
        }

        // multiple delivery partners needed
        else {
            cout << "   Splitting order across stores" << endl;
            map<int, int> allItems; //SKU --> qty

            for(pair<Product*, int>& item: requestedItems) {
                allItems[item.first->getSku()] = item.second;
            }

            int partnerId=1;
            for(DarkStore* store : nearbyDarkStores) {

                if(allItems.empty()) break;

                cout << "   Checking: " << store->getName() << endl;

                bool assigned = false;
                vector<int> toErase;

                for(auto& itemEntry : allItems) {
                    int sku = itemEntry.first;
                    int qtyNeeded = itemEntry.second;
                    int availableQty = store->checkStock(sku);
                    if(availableQty <= 0) continue;

                    // jitna hai utna le lenge
                    int takenQty = min(availableQty, qtyNeeded);
                    store->removeStock(sku, takenQty);

                    cout << "   " << store->getName() << " supplies SKU: " << sku << " x" << takenQty << endl;
                    order->items.push_back({ProductFactory::createProduct(sku), takenQty});

                    //update qty
                    if(qtyNeeded > takenQty) {
                        allItems[sku] = qtyNeeded - takenQty;
                    } else {
                        toErase.push_back(sku);
                    }
                    assigned = true;
                }

                // iteration done let's update the allItems map now
                for(int sku: toErase) allItems.erase(sku);

                // if atleast one SKU is taken then assign delivery partner to it
                if(assigned) {
                    string pname = "Partner" + to_string(partnerId++);
                    order->partners.push_back(new DeliveryPartner(pname));
                    cout << "   Assigned: " << pname << " for " << store->getName() << endl;
                }
            }

            // if still anything remained then show can't be fulfilled
            if(!allItems.empty()) {
                cout << "   Could not fulfill: " << endl;
                for(auto& itemEntry : allItems) {
                    int sku = itemEntry.first;
                    int qty = itemEntry.second;
                    cout << "   SKU: " << sku << " x" << qty << endl;
                }
            }

            // compute totalAmount
            double sum = 0;
            for(auto& item : order->items) {
                sum += item.first->getPrice() * item.second;
            }
            order->totalAmount = sum;
        }

        // order summary
        cout << endl << "[Ordermanager] Order #" << order->orderId<< " Summary: " << endl;
        cout << "   User: " << user->name << endl << "Items:" << endl;
        for (auto& item : order->items) {
            cout << "    SKU " << item.first->getSku()
                 << " (" << item.first->getProductName() << ") x" << item.second
                 << " @ Rs." << item.first->getPrice() << endl;
        }
        cout << "  Total: Rs." << order->totalAmount << endl << "Partners:\n";
        for (auto* dp : order->partners) {
            cout << "    " << dp->name << endl;
        }
        cout << endl;

        orders->push_back(order);

        for (auto* dp : order->partners) delete dp;
        for (auto& item : order->items) delete item.first;
    }

    vector<Order*> getAllOrders() {
        return *orders;
    }

    ~OrderManager() {
        for(auto ord: *orders) {
            delete ord;
        }
        delete orders;
    }
};
OrderManager* OrderManager::instance = nullptr;



// High Level Flow
class Zopto{
public:
    static void showAllItems(User* user) {
        cout << endl << "[Zepto] All available products within 5 KM for " << user->name << endl;

        DarkStoreManager* dsManager = DarkStoreManager::getInstance();
        vector<DarkStore*> nearbyStores = dsManager->getNearbyDarkStores(user->x, user->y, 5.0);

        // collect each sku and it's prices
        map<int, double> skuToPrice;
        map<int, string> skuToName;

        for(DarkStore* darkStore: nearbyStores) {
            vector<Product*> products = darkStore->getAllProducts();

            for(Product* prd: products) {
                int sku = prd->getSku();

                if(skuToPrice.count(sku) == 0){
                    skuToPrice[sku] = prd->getPrice();
                    skuToName[sku] = prd->getProductName();
                }
            }
        }

        for(auto& entry : skuToPrice) {
            int sku = entry.first;
            double price = entry.second;
            cout << "   SKU " << sku << " - " << skuToName[sku] << " @ Rs." << price << endl;
        }
    }

    static void initialize() {
        auto dsManager = DarkStoreManager::getInstance();

        // DarkStore A.......
        DarkStore* darkStoreA = new DarkStore("DarkStoreA", 0.0, 0.0);
        darkStoreA->setReplenishStrategy(new ThresholdReplenishStrategy(3));
 
        cout << endl<< "Adding stocks in DarkStoreA...." << endl;  
        darkStoreA->addStock(101, 5); // Apple
        darkStoreA->addStock(102, 2); // Banana

        // DarkStore B.......
        DarkStore* darkStoreB = new DarkStore("DarkStoreB", 4.0, 1.0);
        darkStoreB->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout << endl << "Adding stocks in DarkStoreB...." << endl; 
        darkStoreB->addStock(101, 3); // Apple
        darkStoreB->addStock(103, 10); // Chocolate

        // DarkStore C.......
        DarkStore* darkStoreC = new DarkStore("DarkStoreC", 2.0, 3.0);
        darkStoreC->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout << endl << "Adding stocks in DarkStoreC...." << endl; 
        darkStoreC->addStock(102, 5); // Banana
        darkStoreC->addStock(201, 7); // T-Shirt

        dsManager->registerDarkStore(darkStoreA);
        dsManager->registerDarkStore(darkStoreB);
        dsManager->registerDarkStore(darkStoreC);
    }
};


int main() {

    // init
    Zopto::initialize();

    // User comes on Platform
    User* user = new User("Ahona", 1.0, 1.0);
    cout <<"\nUser with name " << user->name<< " comes on platform" << endl;

    // Show all available items via Zepto
    Zopto::showAllItems(user);

    // User adds items to cart (some not in a single store)
    cout<<"\nAdding items to cart\n";
    Cart* cart = user->getCart();
    cart->addItem(101, 4);  // dsA has 5, dsB has 3 
    cart->addItem(102, 3);  // dsA has 2, dsC has 5
    cart->addItem(103, 2);  // dsB has 10

    // Place Order
    OrderManager::getInstance()->placeOrder(user, user->cart);

    // Cleanup
    delete user;
    delete DarkStoreManager::getInstance();  // deletes all DarkStores and their inventoryManagers

    return 0;
}