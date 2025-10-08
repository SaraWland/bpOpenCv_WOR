#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Square : public Shape {
public:
    Square();
    ~Square() = default;

    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) override;

private:

};

#endif