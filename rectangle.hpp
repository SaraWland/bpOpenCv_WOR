#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Rectangle : public Shape {
public:
    Rectangle();
    ~Rectangle() = default;

    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage) override;

private:

};

#endif