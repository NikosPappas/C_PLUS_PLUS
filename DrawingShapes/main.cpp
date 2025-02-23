#include <iostream>
#include <vector>
#include <cmath> // For mathematical functions like pow and M_PI
#include <memory> // For smart pointers (unique_ptr)

// Define the Shape interface (Abstract Class)
class Shape {
public:
    // Pure virtual functions that must be implemented by derived classes
    virtual void draw() const = 0;
    virtual double getArea() const = 0;
    virtual double getPerimeter() const = 0;

    // Virtual destructor (important for polymorphism and proper cleanup)
    virtual ~Shape() {
        std::cout << "Shape destructor called (base class)." << std::endl;
    }
};

// Concrete class: Circle
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    void draw() const override {
        std::cout << "Drawing a circle with radius " << radius << std::endl;
    }

    double getArea() const override {
        return M_PI * radius * radius; // M_PI is defined in <cmath>
    }

    double getPerimeter() const override {
        return 2 * M_PI * radius;
    }

    ~Circle() override {
        std::cout << "Circle destructor called." << std::endl;
    }
};

// Concrete class: Rectangle
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void draw() const override {
        std::cout << "Drawing a rectangle with width " << width << " and height " << height << std::endl;
    }

    double getArea() const override {
        return width * height;
    }

    double getPerimeter() const override {
        return 2 * (width + height);
    }

    ~Rectangle() override {
        std::cout << "Rectangle destructor called." << std::endl;
    }
};

// Concrete class: Triangle (using Heron's formula for area)
class Triangle : public Shape {
private:
    double side1;
    double side2;
    double side3;

public:
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {}

    void draw() const override {
        std::cout << "Drawing a triangle with sides " << side1 << ", " << side2 << ", " << side3 << std::endl;
    }

    double getArea() const override {
        // Heron's formula
        double s = (side1 + side2 + side3) / 2.0;
        return sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    double getPerimeter() const override {
        return side1 + side2 + side3;
    }

    ~Triangle() override {
        std::cout << "Triangle destructor called." << std::endl;
    }
};


int main() {
    // Use a vector of unique_ptr to manage the shapes.  This handles memory management automatically.
    std::vector<std::unique_ptr<Shape>> shapes;

    // Add some shapes to the vector
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));

    // Iterate through the vector and draw each shape
    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "Area: " << shape->getArea() << std::endl;
        std::cout << "Perimeter: " << shape->getPerimeter() << std::endl;
        std::cout << std::endl;
    }

    // The unique_ptr objects will automatically be destroyed when the shapes vector goes out of scope.
    // This will call the destructors of the derived classes and the base class, ensuring proper cleanup.

    return 0;
}
