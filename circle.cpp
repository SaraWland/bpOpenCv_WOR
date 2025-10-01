#include "circle.hpp"

Circle::Circle()
{
    this->type = ShapeType::CIRCLE;
}

cv::Mat Circle::findShape(cv::Mat& inputImage)
{
    // TODO implement circle detection using inputImage
    std::cout << "Circle detection not yet implemented." << std::endl;
    return inputImage;
}
