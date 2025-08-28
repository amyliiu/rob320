#include <iostream>
#include <iomanip>

class Felidae {
public:
    Felidae(float weight, float tail_length) : weight(weight), tail_length(tail_length) {
        std::cout << "Felidae constructor" << std::endl;
        std::cout << "Weight: " << weight << " kg" << std::endl;
        std::cout << "Tail length: " << std::fixed << std::setprecision(1) << tail_length << " m" << std::endl;
    }

    ~Felidae() {
        std::cout << "Felidae destructor" << std::endl;
    }

    float weight; // kg
    float tail_length; // m
    virtual void make_sound() = 0;
};

class Panthera : public Felidae {
public:
    Panthera(float weight, float tail_length) : Felidae(weight, tail_length) {
        std::cout << "Panthera constructor" << std::endl;
    }

    ~Panthera() {
        std::cout << "Panthera destructor" << std::endl;
    }

    void make_sound() override {
        std::cout << "Roar" << std::endl;
    }
};

class Tigris : public Panthera {
public:
    Tigris() : Panthera(221.2, 1) {
        std::cout << "Tigris constructor" << std::endl;
    }

    ~Tigris() {
        std::cout << "Tigris destructor" << std::endl;
    }
};

class Felis : public Felidae {
public:
    Felis(float weight, float tail_length) : Felidae(weight, tail_length) {
        std::cout << "Felis constructor" << std::endl;
    }

    ~Felis() {
        std::cout << "Felis destructor" << std::endl;
    }

    void make_sound() override {
        std::cout << "Meow" << std::endl;
    }
};

class Catus : public Felis {
public:
    Catus() : Felis(4.5, 0.3) {
        std::cout << "Catus constructor" << std::endl;
    }

    ~Catus() {
        std::cout << "Catus destructor" << std::endl;
    }
};

int main() {
    Tigris tigris;
    tigris.make_sound();

    Catus catus;
    catus.make_sound();
    return 0;
}