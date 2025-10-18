#include <iostream>

#include "colorManager.hpp"
#include "Logger.hpp"

// cppcheck-suppress unusedFunction ; Used by shape detection classes for creating color mask
cv::Mat ColorManager::getMask(const cv::Mat& inputImage, Color color) const
{
    if (inputImage.empty()) {
        return cv::Mat::zeros(480, 640, CV_8UC1);
    }

    cv::Mat hsv, mask;
    cv::cvtColor(inputImage, hsv, cv::COLOR_BGR2HSV);
    
    switch (color) {
    case Color::PINK:
    {
        cv::Mat mask1, mask2;
        // Range 1: Magenta-pink (150-180)
        cv::inRange(hsv, cv::Scalar(150, 50, 50), cv::Scalar(180, 255, 255), mask1);
        // Range 2: Red-pink (0-10)
        cv::inRange(hsv, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), mask2);
        // Combine both ranges
        cv::bitwise_or(mask1, mask2, mask);
        // Logger::getInstance().log("Roze geselecteerd.\n");
        break;
    }
    case Color::GREEN:
    {
        cv::Mat mask1, mask2;
        // Range 1: Pure green (40-70)
        cv::inRange(hsv, cv::Scalar(32, 80, 50), cv::Scalar(70, 255, 255), mask1);
        // Range 2: Lime green (70-85) - separate for different green shades
        cv::inRange(hsv, cv::Scalar(70, 60, 50), cv::Scalar(85, 255, 255), mask2);
        // Combine both green ranges
        cv::bitwise_or(mask1, mask2, mask);
        // Logger::getInstance().log("Groen geselecteerd.\n");
        break;
    }
    case Color::YELLOW:
    {
        cv::inRange(hsv, cv::Scalar(23, 100, 100), cv::Scalar(31, 255, 255), mask);
        // Logger::getInstance().log("Geel geselecteerd.\n");
        break;
    }
    case Color::ORANGE:
    {
        cv::inRange(hsv, cv::Scalar(10, 100, 100), cv::Scalar(23, 255, 255), mask);
        // Logger::getInstance().log("Oranje geselecteerd.\n");
        break;
    }
    default:
    {
        mask = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC1);
        break;
    }
    }

    // Reduce noise
    cv::Mat kernel;
    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);  
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);   
    
    return mask;
}
