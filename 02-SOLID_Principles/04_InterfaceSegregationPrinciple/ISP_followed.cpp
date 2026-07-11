#include <iostream>
using namespace std;

class TwoDShape {
    public:
        virtual double area() = 0;
};

class ThreeDShape{
    public:
        virtual double area() = 0;
        virtual double volume() = 0;
};

class Square : public TwoDShape {
private:
    double side;
public:
    Square(double s) : side(s) {}

    double area() override {
        return side * side;
    }
};

class Cube : public ThreeDShape {
    private:
    double side;
    public:
    Cube(double s) : side (s) {}

    double area() override {
        return side * side;
    }

    double volume() override {
        return side * side * side;
    }
};

int main() {
    TwoDShape* square = new Square(5);
    ThreeDShape* cube = new Cube(3);

    cout << "Square Area: " << square->area() << endl;
    cout << "Cube Area: " << cube->area() << endl; 

    cout << "Cube volume: " << cube->volume() << endl;    
    return 0;
}