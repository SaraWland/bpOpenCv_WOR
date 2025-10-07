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
        {
            // Create three masks for different green ranges and combine them
            cv::Mat mask1, mask2, mask3;
            // Very light green range
            cv::inRange(hsv, cv::Scalar(30, 15, 15), cv::Scalar(90, 255, 255), mask1);
            // Medium green range
            cv::inRange(hsv, cv::Scalar(35, 25, 25), cv::Scalar(85, 255, 255), mask2);
            // Dark green range
            cv::inRange(hsv, cv::Scalar(40, 30, 30), cv::Scalar(80, 255, 180), mask3);
            cv::Mat tempMask;
            cv::bitwise_or(mask1, mask2, tempMask);
            cv::bitwise_or(tempMask, mask3, mask);
            std::cout << "Green color mask created with triple range." << std::endl;

            // cv::Mat bright, dark;
            // cv::inRange(hsv, cv::Scalar(35, 25, 150), cv::Scalar(85, 255, 255), bright);
            // cv::inRange(hsv, cv::Scalar(35, 25, 25),  cv::Scalar(85, 255, 150), dark);
            // cv::bitwise_or(bright, dark, mask);


            // cv::inRange(hsv, cv::Scalar(35, 25, 25), cv::Scalar(85, 255, 255), mask);
            // std::cout << "Green color mask created with single range." << std::endl;
        }
        break;
    case Color::YELLOW:
        cv::inRange(hsv, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), mask);
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

    // Reduce noise
    cv::Mat kernel;
    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);  
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);   
    
    return mask;
}
