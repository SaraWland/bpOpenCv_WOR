#include "square.hpp"

Square::Square()
{
    this->type = ShapeType::SQUARE;
}

cv::Mat Square::findShape(cv::Mat& inputImage)
{
    // TODO implement square detection using inputImage
    std::cout << "Square detection not yet implemented." << std::endl;
    return inputImage;
}