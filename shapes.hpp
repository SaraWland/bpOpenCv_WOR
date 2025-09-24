#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "enums.hpp"


class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual void findShape(Color color) = 0;

    ShapeType getType() const { return type; }
protected:
    ShapeType type;
};

#endif