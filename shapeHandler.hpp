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

    /**
     * @brief Calls the appropriate shape detection method based on the provided shape type.
     * 
     * @param inputImage Reference to the pre-processed image used for detection.
     * @param shapeType ShapeType enum indicating which shape to detect.
     * @param originalImage Reference to the original image for displaying results.
     * @param contourImage Reference to the image for drawing contours.
     * @param isInteractive Boolean indicating if the mode is interactive.
     * @return cv::Mat The output image with detected shapes.
     */
    cv::Mat detectShape(cv::Mat& inputImage, ShapeType shapeType, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive);

private:
    std::vector<std::unique_ptr<Shape>> shapes;

};

#endif