#include <iostream>
using namespace std;

class Shape {
    public:
        virtual double area() = 0;
        virtual double volume() = 0; // applies for 3D shapes only
};

class Square : public Shape {
private:
    double side;
public:
    Square(double s) : side(s) {}

    double area() override {
        return side * side;
    }

    double volume() override {
        throw logic_error("Volume is not applicable for this shape");
    }
};

class Cube : public Shape {
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
    Shape* square = new Square(5);
    Shape* cube = new Cube(3);

    cout << "Square Area: " << square->area() << endl;
    cout << "Cube Area: " << cube->area() << endl;

    try
    {
        cout << "Square Volume: " << square->volume() << endl; // this will throw exception
    }
    catch(logic_error& e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    cout << "Cube volume: " << cube->volume() << endl;    
    return 0;
}