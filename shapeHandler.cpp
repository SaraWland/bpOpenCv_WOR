#include "shapeHandler.hpp"
#include "circle.hpp"
#include "rectangle.hpp"
#include "triangle.hpp"
#include "square.hpp"
#include "halfCircle.hpp"

ShapeHandler::ShapeHandler()
{
    shapes.reserve(5);
    shapes.push_back(std::make_unique<Circle>());
    shapes.push_back(std::make_unique<Rectangle>());
    shapes.push_back(std::make_unique<Triangle>()); 
    shapes.push_back(std::make_unique<Square>());
    shapes.push_back(std::make_unique<HalfCircle>());
}