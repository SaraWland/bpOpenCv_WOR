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
        cv::inRange(hsv, cv::Scalar(150, 30, 50), cv::Scalar(170, 255, 255), mask);
        std::cout << "Pink color mask created." << std::endl;
        break;
    case Color::GREEN:
        cv::inRange(hsv, cv::Scalar(45, 80, 80), cv::Scalar(70, 255, 255), mask);
        std::cout << "Green color mask created." << std::endl;
        break;
    case Color::YELLOW:
        cv::inRange(hsv, cv::Scalar(15, 120, 150), cv::Scalar(35, 255, 255), mask);
        std::cout << "Yellow color mask created." << std::endl;
        break;
    case Color::ORANGE:
        cv::inRange(hsv, cv::Scalar(10, 100, 100), cv::Scalar(25, 255, 255), mask);
        std::cout << "Orange color mask created." << std::endl;
        break;
    default:
        mask = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC1);
        break;
    }
    return mask;
}
