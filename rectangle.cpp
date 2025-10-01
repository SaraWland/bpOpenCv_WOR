#include "rectangle.hpp"

Rectangle::Rectangle()
{
    this->type = ShapeType::RECTANGLE;
}

cv::Mat Rectangle::findShape(cv::Mat& inputImage)
{
    // TODO implement rectangle detection using inputImage
    std::cout << "Rectangle detection not yet implemented." << std::endl;
    return inputImage;
}