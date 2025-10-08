#ifndef HALF_CIRCLE_HPP
#define HALF_CIRCLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class HalfCircle : public Shape {
public:
    HalfCircle() = default;
    ~HalfCircle() = default;

    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) override;

private:

};

#endif