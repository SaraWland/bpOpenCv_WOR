#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Triangle : public Shape {
public:
    Triangle() = default;
    ~Triangle() = default;

    /**
     * @brief Finds triangles in the input image.
     * 
     * @param inputImage Reference to the pre-processed image used for detection.
     * @param originalImage Reference to the original image for visualization.
     * @param contourImage Reference to the contour image for visualization.
     * @param isInteractive Boolean indicating if the mode is interactive.
     * @return cv::Mat Image with detected shapes and information drawn.
     */
    cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) override;
private:

};

#endif