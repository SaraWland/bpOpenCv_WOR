#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Rectangle : public Shape {
public:
    Rectangle();
    ~Rectangle() = default;

    void findShape(Color color) override; //TODO change return type

private:

};

#endif