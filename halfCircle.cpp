#include "halfCircle.hpp"
#include <iostream>
#include <vector>

HalfCircle::HalfCircle()
{
    this->type = ShapeType::HALF_CIRCLE;
}

cv::Mat HalfCircle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;

    cv::GaussianBlur(inputImage, processableImage, cv::Size(3,3), 1, 1);
    
    // Find all contours directly
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point>> validHalfCircles;
    
    // Check each contour for half-circle properties
    for (size_t i = 0; i < contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);
        if (area < 800 || area > 8000) continue;
        
        cv::Moments M = cv::moments(contours[i]);
        if (M.m00 == 0) continue;
        
        // Check circularity for half circles
        double perimeter = cv::arcLength(contours[i], true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);
        
        // Additional check: convex hull ratio for half-circles
        std::vector<cv::Point> hull;
        cv::convexHull(contours[i], hull);
        double hullArea = cv::contourArea(hull);
        double convexity = area / hullArea;
        
        std::cout << "Contour " << i << ": area=" << area << ", circularity=" << circularity << ", convexity=" << convexity << std::endl;
        
        // Very strict criteria for half-circles
        if (circularity > 0.45 && circularity < 0.62 && convexity > 0.85)
        {
            validHalfCircles.push_back(contours[i]);
            std::cout << "  -> Accepted as half-circle" << std::endl;
        }
    }
    
    for (size_t i = 0; i < validHalfCircles.size(); ++i)
    {
        // Safety check
        if (validHalfCircles[i].empty()) continue;
        
        // Calculate centroid for logging
        cv::Moments M = cv::moments(validHalfCircles[i]);
        if (M.m00 == 0) continue;
        
        cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
        double area = cv::contourArea(validHalfCircles[i]);
        
        // Draw the actual half-circle shape
        cv::polylines(outputImage, validHalfCircles[i], true, cv::Scalar(255, 165, 0), 2);
        cv::circle(outputImage, center, 3, cv::Scalar(0, 0, 255), -1);
        
        // Draw on contour image
        cv::polylines(contourImage, validHalfCircles[i], true, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);
        
        std::cout << "Half-circle " << i << ": Center = (" << center.x << ", " << center.y << "), Area = " << static_cast<int>(area) << std::endl;
    }

    return outputImage;
}