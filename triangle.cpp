#include "triangle.hpp"
#include <iostream>
#include <vector>

Triangle::Triangle()
{
    this->type = ShapeType::TRIANGLE;
}

cv::Mat Triangle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;
    
    cv::GaussianBlur(inputImage, processableImage, cv::Size(5, 5), 1, 1);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    int triangleCount = 0;
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);
        if (area < 100 || area > 8000) 
            continue;
            
        // convert contour to polygon
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx, epsilon, true);

        // Check if the polygon is a triangle
        if (approx.size() == 3)
        {
            // Calculate center
            cv::Moments M = cv::moments(contours[i]);
            if (M.m00 != 0) 
            {
                cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
                
                // Draw the triangle
                cv::polylines(outputImage, approx, true, cv::Scalar(0, 255, 0), 2);
                cv::circle(outputImage, center, 3, cv::Scalar(0, 0, 255), -1);
                
                // Draw contours on black background for visualization
                cv::polylines(contourImage, approx, true, cv::Scalar(255, 255, 255), 2);
                cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);
                
                // Log triangle details
                std::cout << "Triangle " << triangleCount << ": Center = (" << center.x << ", " << center.y 
                          << "), Area = " << static_cast<int>(area) << std::endl;
                triangleCount++;
            }
        }
    }
    
    return outputImage;
}
