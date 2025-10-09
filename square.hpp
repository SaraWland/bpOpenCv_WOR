#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "shapes.hpp"
#include "enums.hpp"

class Square : public Shape {
public:
    Square() = default;
    ~Square() override = default;

    /**
     * @brief Finds squares in the input image.
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