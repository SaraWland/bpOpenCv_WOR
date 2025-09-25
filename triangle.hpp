#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Triangle : public Shape {
public:
    Triangle();
    ~Triangle() = default;

    void findShape(cv::Mat& inputImage) override; //TODO change return type

private:

};

#endif