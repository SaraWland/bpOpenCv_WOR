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

    /**
     * @brief Finds shapes in the input image.
     * 
     * @param inputImage Reference to the pre-processed image used for detection.
     * @param originalImage Reference to the original image for visualization.
     * @param contourImage Reference to the contour image for visualization.
     * @param isInteractive Boolean indicating if the modus is interactive.
     * @return cv::Mat Image with detected shapes and information drawn.
     */
    virtual cv::Mat findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive) = 0;
};

#endif