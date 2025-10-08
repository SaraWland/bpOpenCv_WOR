#ifndef COLOR_MANAGER_HPP
#define COLOR_MANAGER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "enums.hpp"

class ColorManager {
public:
    ColorManager() = default;
    ~ColorManager() = default;

    /**
     * @brief Creates a mask for a specific color in the input image.
     * 
     * @param inputImage Reference to the input image.
     * @param aColor The color to create a mask for.
     * @return cv::Mat Binary mask with the specified color.
     */
    cv::Mat getMask(const cv::Mat& inputImage, Color aColor) const;

private:
};

#endif