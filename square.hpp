#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Square : public Shape {
public:
    Square();
    ~Square() = default;

    void findShape(cv::Mat& inputImage) override; //TODO change return type

private:

};

#endif