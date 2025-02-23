#include <iostream>
#include <string>
#include <random> // For random number generation
#include <vector>
#include <memory> // For unique_ptr

// Interface (Abstract Class): Animal
class Animal {
public:
    // Pure virtual functions that derived classes must implement
    virtual std::string makeSound() const = 0;
    virtual void move() = 0; // Movement is now not const since it changes the animal's state

    // Virtual destructor: Crucial for polymorphism and proper cleanup
    virtual ~Animal() {
        std::cout << "Animal destructor called (base class)." << std::endl;
    }
};

// Concrete Class: Dog
class Dog : public Animal {
private:
    std::string name;
    int position; // Current position of the dog in the simulation

public:
    Dog(const std::string& n) : name(n), position(0) {
        std::cout << "Dog " << name << " created." << std::endl;
    }

    std::string makeSound() const override {
        return "Woof!";
    }

    void move() override {
        // Simulate random movement
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(-2, 2); // Move -2 to +2 units
        int moveAmount = distrib(gen);
        position += moveAmount;
        std::cout << "Dog " << name << " moved to position " << position << std::endl;
    }

    ~Dog() override {
        std::cout << "Dog " << name << " destructor called." << std::endl;
    }
};

// Concrete Class: Cat
class Cat : public Animal {
private:
    std::string name;
    int position;

public:
    Cat(const std::string& n) : name(n), position(0) {
        std::cout << "Cat " << name << " created." << std::endl;
    }

    std::string makeSound() const override {
        return "Meow!";
    }

    void move() override {
        // Simulate random movement
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(-3, 3); // Cats are slightly faster
        int moveAmount = distrib(gen);
        position += moveAmount;
        std::cout << "Cat " << name << " moved to position " << position << std::endl;
    }

    ~Cat() override {
        std::cout << "Cat " << name << " destructor called." << std::endl;
    }
};

// Concrete Class: Bird
class Bird : public Animal {
private:
    std::string name;
    int position;

public:
    Bird(const std::string& n) : name(n), position(0) {
        std::cout << "Bird " << name << " created." << std::endl;
    }

    std::string makeSound() const override {
        return "Chirp!";
    }

    void move() override {
        // Simulate random movement (birds can fly further)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(-5, 5); // Birds can move more
        int moveAmount = distrib(gen);
        position += moveAmount;
        std::cout << "Bird " << name << " flew to position " << position << std::endl;
    }

    ~Bird() override {
        std::cout << "Bird " << name << " destructor called." << std::endl;
    }
};

int main() {
    // Use a vector of unique_ptr to manage the animals
    std::vector<std::unique_ptr<Animal>> animals;

    // Create some animals and add them to the vector
    animals.push_back(std::make_unique<Dog>("Buddy"));
    animals.push_back(std::make_unique<Cat>("Whiskers"));
    animals.push_back(std::make_unique<Bird>("Tweety"));

    // Simulate the animal ecosystem for a few rounds
    for (int round = 1; round <= 5; ++round) {
        std::cout << "--- Round " << round << " ---" << std::endl;
        for (const auto& animal : animals) {
            std::cout << animal->makeSound() << std::endl;
            animal->move();
        }
        std::cout << std::endl;
    }

    // The unique_ptr objects will automatically be destroyed when the animals vector goes out of scope,
    // calling the destructors of the derived classes and the base class.

    return 0;
}
