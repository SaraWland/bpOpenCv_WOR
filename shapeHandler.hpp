#ifndef SHAPE_HANDLER_HPP
#define SHAPE_HANDLER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <memory>
#include "shapes.hpp"

class ShapeHandler {
public:
    ShapeHandler();
    ~ShapeHandler();

    cv::Mat detectShape(cv::Mat& inputImage, ShapeType shapeType);

private:
    std::vector<std::unique_ptr<Shape>> shapes;

};

#endif