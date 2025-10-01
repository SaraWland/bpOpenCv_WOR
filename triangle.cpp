#include "triangle.hpp"

Triangle::Triangle()
{
    this->type = ShapeType::TRIANGLE;
}

cv::Mat Triangle::findShape(cv::Mat& inputImage)
{
    // TODO implement triangle detection using inputImage
    std::cout << "Triangle detection not yet implemented." << std::endl;
    return inputImage;
}
