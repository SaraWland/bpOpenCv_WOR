#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Circle : public Shape {
public:
    Circle() = default;
    ~Circle() = default;

    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) override;

private:

};

#endif