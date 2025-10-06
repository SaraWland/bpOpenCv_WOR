#include "circle.hpp"
#include <iostream>
#include <vector>

Circle::Circle()
{
    this->type = ShapeType::CIRCLE;
}

cv::Mat Circle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;

    // Reduce blur for better edge detection
    cv::GaussianBlur(inputImage, processableImage, cv::Size(3,3), 1, 1);
    
    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(processableImage, circles, cv::HOUGH_GRADIENT, 1, 25, 50, 15, 10, 120);
    
    std::vector<cv::Vec3f> completeCircles;
    
    // Filter out half circles by checking contour completeness
    for (size_t i = 0; i < circles.size(); ++i)
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
        // Find contours in the circle area
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        
        bool isCompleteCircle = false;
        
        // Check if any contour is close to this circle and has circular shape
        for (size_t j = 0; j < contours.size(); ++j)
        {
            double area = cv::contourArea(contours[j]);
            if (area < 300) continue;
            
            cv::Moments M = cv::moments(contours[j]);
            if (M.m00 == 0) continue;
            
            cv::Point contourCenter(M.m10 / M.m00, M.m01 / M.m00);
            double distance = cv::norm(center - contourCenter);
            
            // If contour center is close to circle center
            if (distance < radius * 0.3)
            {
                // Check circularity: area vs perimeter ratio
                double perimeter = cv::arcLength(contours[j], true);
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);
                
                // Complete circles have circularity close to 1, half circles much less
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
        cv::Point center(cvRound(completeCircles[i][0]), cvRound(completeCircles[i][1]));
        int radius = cvRound(completeCircles[i][2]);
        cv::circle(outputImage, center, radius, cv::Scalar(0, 255, 0), 2);
        cv::circle(outputImage, center, 2, cv::Scalar(0, 0, 255), 3);
        
        // Draw circles on black background for visualization
        cv::circle(contourImage, center, radius, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 2, cv::Scalar(0, 255, 0), 3);
        
        // Log circle details
        std::cout << "Circle " << i << ": Center = (" << center.x << ", " << center.y << "), Radius = " << radius << std::endl;
    }

    return outputImage;
}
