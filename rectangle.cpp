#include "rectangle.hpp"

Rectangle::Rectangle()
{
    this->type = ShapeType::RECTANGLE;
}

cv::Mat Rectangle::findShape(cv::Mat& inputImage, cv::Mat& originalImage)
{
    // TODO implement rectangle detection using inputImage
    std::cout << "Rectangle detection not yet implemented." << std::endl;
    return inputImage;
}