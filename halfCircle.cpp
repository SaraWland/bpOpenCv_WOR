#include "halfCircle.hpp"   

HalfCircle::HalfCircle()
{
    this->type = ShapeType::HALF_CIRCLE;
}

cv::Mat HalfCircle::findShape(cv::Mat& inputImage)
{
    // TODO implement half-circle detection using inputImage
    std::cout << "Half-circle detection not yet implemented." << std::endl;
    return inputImage;
}