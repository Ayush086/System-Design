/* Virtual Porxy Design Pattern                                                                                                    20/06/2026  */
#include <iostream>
#include <string>
using namespace std;

class IImage{
public:
    virtual void display() = 0;
    virtual ~IImage() = default;
};

class ImageDisplay : public IImage {
private:
    string path;

public:
    ImageDisplay(string file) {
        this->path = file;
        // heavy operation
        cout << "[ImageDisplay] Performing heavy operations in background for " << path << endl;
    }

    void display() override {
        cout << "[ImageDisplay] Displaying Image: " << path << endl;
    }
};

class ImageProxy : public IImage {
private:
    ImageDisplay* realImage;
    string path;
public:
    ImageProxy(string file) {
        this->path = file;
        realImage = nullptr;
    }

    void display() override {
        // lazy image init
        if(realImage == nullptr) {
            realImage = new ImageDisplay(path);
        }
        realImage->display();
    }
};


int main() {
    IImage* img1 = new ImageProxy("example.jpeg");

    img1->display();

    return 0;
}