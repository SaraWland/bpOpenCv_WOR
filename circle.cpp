#include "circle.hpp"
#include <iostream>
#include <vector>
#include "Logger.hpp"

Circle::Circle()
{
    this->type = ShapeType::CIRCLE;
}

cv::Mat Circle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;

    cv::GaussianBlur(inputImage, processableImage, cv::Size(3,3), 1, 1);
    
    // Find circles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(processableImage, circles, cv::HOUGH_GRADIENT, 1, 25, 50, 15, 10, 120);
    
    std::vector<cv::Vec3f> completeCircles;
    
    for (size_t i = 0; i < circles.size(); ++i)
    {
        // Calculate center and radius
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
        // Find contours in the circle area
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        
        bool isCompleteCircle = false;
        
        for (size_t j = 0; j < contours.size(); ++j)
        {
            double area = cv::contourArea(contours[j]);
            // Avoid small noise areas
            if (area < 300) 
            {
                continue;
            }

            cv::Moments M = cv::moments(contours[j]);
            // .m00 is the area of the contour
            if (M.m00 == 0) 
            {
                continue;
            }
            cv::Point contourCenter(M.m10 / M.m00, M.m01 / M.m00);
            double distance = cv::norm(center - contourCenter);
            
            // If contour center is close to circle center
            // Multiplying the radius creates tolerance
            if (distance < radius * 0.3)
            {
                // Circularity check to filter out half-circles
                // 4 * PI * area / (perimeter * perimeter) 
                double perimeter = cv::arcLength(contours[j], true);
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);
                
                // Below a certain value the shape is not circular enough
                if (circularity > 0.7)
                {
                    isCompleteCircle = true;
                    break;
                }
            }
        }
        
        if (isCompleteCircle)
        {
            completeCircles.push_back(circles[i]);
        }
    }
    
    for (size_t i = 0; i < completeCircles.size(); ++i)
    {
        // Draw the circle
        cv::Point center(cvRound(completeCircles[i][0]), cvRound(completeCircles[i][1]));
        int radius = cvRound(completeCircles[i][2]);
        cv::circle(outputImage, center, radius, cv::Scalar(0, 255, 0), 2);
        cv::circle(outputImage, center, 2, cv::Scalar(0, 0, 255), 3);
        
        // Draw circles on black background for visualization
        cv::circle(contourImage, center, radius, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 2, cv::Scalar(0, 255, 0), 3);
        
        // Log circle details
        Logger::getInstance().log("Circle detected at (" + std::to_string(center.x) + ", " + std::to_string(center.y) + ") with radius " + std::to_string(radius));
    }

    return outputImage;
}
