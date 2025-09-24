#ifndef SHAPE_HANDLER_HPP
#define SHAPE_HANDLER_HPP

#include <vector>
#include <memory>
#include "shapes.hpp"

class ShapeHandler {
public:
    ShapeHandler();
    ~ShapeHandler();

private:
    std::vector<std::unique_ptr<Shape>> shapes;

};

#endif