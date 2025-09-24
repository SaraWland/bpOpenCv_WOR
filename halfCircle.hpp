#ifndef HALF_CIRCLE_HPP
#define HALF_CIRCLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class HalfCircle : public Shape {
public:
    HalfCircle();
    ~HalfCircle() = default;

    void findShape(Color color) override; //TODO change return type

private:

};

#endif