#include <iostream>

#include "colorManager.hpp"



cv::Mat ColorManager::getMask(const cv::Mat& inputImage, Color color) const
{
    if (inputImage.empty()) {
        return cv::Mat::zeros(480, 640, CV_8UC1);
    }

    cv::Mat hsv, mask;
    cv::cvtColor(inputImage, hsv, cv::COLOR_BGR2HSV);
    
    switch (color) {
    case Color::PINK:
        // HSV range for pink: Hue ~150-170, Saturation 30-255, Value 50-255
        cv::inRange(hsv, cv::Scalar(150, 30, 50), cv::Scalar(170, 255, 255), mask);
        std::cout << "Pink color mask created." << std::endl;
        break;
    case Color::GREEN:
        // HSV range for green: Hue ~60-80, Saturation 50-255, Value 50-255
        cv::inRange(hsv, cv::Scalar(60, 50, 50), cv::Scalar(80, 255, 255), mask);
        std::cout << "Green color mask created." << std::endl;
        break;
    case Color::YELLOW:
        // HSV range for yellow: Hue ~20-30, Saturation 100-255, Value 100-255
        cv::inRange(hsv, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), mask);
        std::cout << "Yellow color mask created." << std::endl;
        break;
    case Color::ORANGE:
        // HSV range for orange: Hue ~10-25, Saturation 100-255, Value 100-255
        cv::inRange(hsv, cv::Scalar(10, 100, 100), cv::Scalar(25, 255, 255), mask);
        std::cout << "Orange color mask created." << std::endl;
        break;
    default:
        mask = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC1);
        break;
    }
    return mask;
}
