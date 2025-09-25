#ifndef COLOR_MANAGER_HPP
#define COLOR_MANAGER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "enums.hpp"

class ColorManager {
public:
    ColorManager() = default;
    ~ColorManager() = default;

    cv::Mat getMask(const cv::Mat& inputImage, Color aColor) const;

private:
};

#endif