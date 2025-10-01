#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "enums.hpp"


class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual cv::Mat findShape(cv::Mat& inputImage) = 0;

    ShapeType getType() const { return type; }
protected:
    ShapeType type;
};

#endif