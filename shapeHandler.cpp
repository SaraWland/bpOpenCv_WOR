#include "shapeHandler.hpp"
#include "circle.hpp"
#include "rectangle.hpp"
#include "triangle.hpp"
#include "square.hpp"
#include "halfCircle.hpp"
#include "colorManager.hpp"

ShapeHandler::ShapeHandler()
{
    shapes.reserve(5);
    shapes.push_back(std::make_unique<Circle>());
    shapes.push_back(std::make_unique<Rectangle>());
    shapes.push_back(std::make_unique<Triangle>()); 
    shapes.push_back(std::make_unique<Square>());
    shapes.push_back(std::make_unique<HalfCircle>());
}

ShapeHandler::~ShapeHandler()
{
    shapes.clear();
}

cv::Mat ShapeHandler::detectShape(cv::Mat& colorMask, ShapeType shapeType, cv::Mat& originalImage, cv::Mat& contourImage)
{
    contourImage = cv::Mat::zeros(originalImage.size(), CV_8UC3);
    
    cv::Mat processedImage;
    switch (shapeType)
    {
    case ShapeType::CIRCLE:
        processedImage = shapes.at(0)->findShape(colorMask, originalImage, contourImage);
        break;
    case ShapeType::RECTANGLE:
        processedImage = shapes.at(1)->findShape(colorMask, originalImage, contourImage);
        break;
    case ShapeType::TRIANGLE:
        processedImage = shapes.at(2)->findShape(colorMask, originalImage, contourImage);
        break;
    case ShapeType::SQUARE:
        processedImage = shapes.at(3)->findShape(colorMask, originalImage, contourImage);
        break;
    case ShapeType::HALF_CIRCLE:
        processedImage = shapes.at(4)->findShape(colorMask, originalImage, contourImage);
        break;
    default:
        break;
    }
    return processedImage;
}