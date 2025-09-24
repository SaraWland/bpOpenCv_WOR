#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Circle : public Shape {
public:
    Circle();
    ~Circle() = default;

    void findShape(Color color) override; //TODO change return type

private:

};

#endif