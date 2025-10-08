#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Triangle : public Shape {
public:
    Triangle() = default;
    ~Triangle() = default;

    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) override;
private:

};

#endif