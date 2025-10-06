#include "rectangle.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

Rectangle::Rectangle()
{
    this->type = ShapeType::RECTANGLE;
}

cv::Mat Rectangle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;
    
    // Apply Gaussian blur to reduce noise
    cv::GaussianBlur(inputImage, processableImage, cv::Size(5, 5), 1, 1);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    int rectangleCount = 0;
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Filter by contour area to avoid noise
        double area = cv::contourArea(contours[i]);
        if (area < 100 || area > 8000) 
        {
            continue;
        }

        // convert contour to polygon
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx, epsilon, true);
        
        // Check if the polygon is a rectangle (4 vertices)
        if (approx.size() == 4)
        {
            // Check if it's a rectangle but NOT a square by comparing side lengths
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
            
            if (ratio >= 1.3) // Rectangles have unequal sides, squares are excluded
            {
                // Calculate centroid
                cv::Moments M = cv::moments(contours[i]);
                if (M.m00 != 0)
                {
                    cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
                    
                    // Draw the rectangle
                    cv::polylines(outputImage, approx, true, cv::Scalar(0, 255, 255), 2);
                    cv::circle(outputImage, center, 3, cv::Scalar(0, 0, 255), -1); 
                    
                    // Draw contours on black background for visualization
                    cv::polylines(contourImage, approx, true, cv::Scalar(255, 255, 255), 2);
                    cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);
                    
                    // Log rectangle details
                    std::cout << "Rectangle " << rectangleCount << ": Center = (" << center.x << ", " << center.y 
                              << "), Area = " << static_cast<int>(area) 
                              << ", Side ratio = " << std::fixed << std::setprecision(2) << ratio << std::endl;
                    rectangleCount++;
                }
            }
        }
    }
    
    return outputImage;
}