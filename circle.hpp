#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Circle : public Shape {
public:
    Circle() = default;
    ~Circle() override = default;

    /**
     * @brief Finds circles in the input image.
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