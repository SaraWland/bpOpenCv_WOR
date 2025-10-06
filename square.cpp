#include "square.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "Logger.hpp"

Square::Square()
{
    this->type = ShapeType::SQUARE;
}

cv::Mat Square::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;
    
    cv::GaussianBlur(inputImage, processableImage, cv::Size(5, 5), 1, 1);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    int squareCount = 0;
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Avoid big areas and small noise areas
        double area = cv::contourArea(contours[i]);
        if (area < 100 || area > 15000)
        {
            continue;
        }

        // convert contour to polygon for corner checking
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx, epsilon, true);
                
        // Check if the polygon is a square
        if (approx.size() == 4)
        {
            
            // Check if it's approximately a square by comparing side lengths
            std::vector<double> sideLengths;
            for (int j = 0; j < 4; j++)
            {
                cv::Point p1 = approx[j];
                cv::Point p2 = approx[(j + 1) % 4];
                double length = cv::norm(p1 - p2);
                sideLengths.push_back(length);
            }
            
            // Calculate the ratio between longest and shortest side
            double minSide = *std::min_element(sideLengths.begin(), sideLengths.end());
            double maxSide = *std::max_element(sideLengths.begin(), sideLengths.end());
            double ratio = maxSide / minSide;
                        
            // If ratio is close to 1, it's approximately a square
            if (ratio < 1.3)
            {
                // Calculate center
                cv::Moments M = cv::moments(contours[i]);
                // .m00 is the area of the contour
                if (M.m00 != 0)
                {
                    cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
                    
                    // Draw the square
                    cv::polylines(outputImage, approx, true, cv::Scalar(255, 0, 0), 2);
                    cv::circle(outputImage, center, 3, cv::Scalar(0, 0, 255), -1);
                    
                    // Draw contours on black background for visualization
                    cv::polylines(contourImage, approx, true, cv::Scalar(255, 255, 255), 2);
                    cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);
                    
                    // Log square details
                    Logger::getInstance().log("Square detected at (" + std::to_string(center.x) + ", " + std::to_string(center.y) + ") with area " + std::to_string(static_cast<int>(area)) + ") and side ratio " + std::to_string(ratio));
                    
                    squareCount++;
                }
            }
        }
    }
    
    return outputImage;
}