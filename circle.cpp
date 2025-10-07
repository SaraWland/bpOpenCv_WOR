#include "circle.hpp"
#include <iostream>
#include <vector>
#include <chrono>
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
    
    // Find circles (expensive operation, not timed individually)
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(processableImage, circles, cv::HOUGH_GRADIENT, 1, 25, 50, 15, 10, 120);
    
    // Find contours once for all circle validation
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // Start timing for detection process
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    
    std::vector<cv::Vec3f> completeCircles;
    std::vector<long long> individualDetectionTimes; // Store individual detection times
    
    for (size_t i = 0; i < circles.size(); ++i)
    {
        // Start timing for individual circle validation
        std::chrono::high_resolution_clock::time_point individualStartTime = std::chrono::high_resolution_clock::now();

        // Calculate center and radius
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
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
            // End timing for individual circle validation
            std::chrono::high_resolution_clock::time_point individualEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<long long, std::nano> individualDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(individualEndTime - individualStartTime);
            
            // Convert nanoseconds to actual clock ticks using approximate CPU frequency
            double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
            long long individualClockTicks = static_cast<long long>(individualDuration.count() * cpuFrequencyGHz);
            
            completeCircles.push_back(circles[i]);
            individualDetectionTimes.push_back(individualClockTicks);
        }
    }
    
    // End timing for detection process
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    
    // Convert nanoseconds to actual clock ticks using approximate CPU frequency
    double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
    long long totalClockTicks = static_cast<long long>(duration.count() * cpuFrequencyGHz);
    
    for (size_t i = 0; i < completeCircles.size(); ++i)
    {
        // Draw the circle
        cv::Point center(cvRound(completeCircles[i][0]), cvRound(completeCircles[i][1]));
        int radius = cvRound(completeCircles[i][2]);
        double area = CV_PI * radius * radius;
        
        cv::circle(outputImage, center, radius, cv::Scalar(0, 255, 0), 2);
        cv::circle(outputImage, center, 2, cv::Scalar(0, 0, 255), 3);
        
        // Draw coords, area and individual clock ticks on output image
        // make sure text is within image bounds and does not overlap with the shape
        std::string info = "X,Y(" + std::to_string(center.x) + ", " + std::to_string(center.y) + "), A: " + std::to_string(static_cast<int>(area)) + ", T: " + std::to_string(individualDetectionTimes[i]) + " ticks";
        
        // Calculate text size to ensure it fits
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(info, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseline);
        
        // Start with default position (right and above the center)
        cv::Point textPos(center.x + radius + 10, center.y - 10);
        
        // Check if text goes beyond right edge
        if (textPos.x + textSize.width > outputImage.cols) {
            textPos.x = center.x - radius - textSize.width - 10; // Move to left side
        }
        
        // Check if text goes beyond left edge
        if (textPos.x < 0) {
            textPos.x = 10; // Move to left margin
        }
        
        // Check if text goes above top edge
        if (textPos.y - textSize.height < 0) {
            textPos.y = center.y + radius + textSize.height + 10; // Move below shape
        }
        
        // Check if text goes below bottom edge
        if (textPos.y > outputImage.rows) {
            textPos.y = outputImage.rows - 10; // Move to bottom margin
        }
        
        cv::putText(outputImage, info, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 0), 1);
        
        // Draw circles on black background for visualization
        cv::circle(contourImage, center, radius, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 2, cv::Scalar(0, 255, 0), 3);
    }

    // Always display detection timing information
    if (completeCircles.size() == 0){
        std::string noDetectionMsg = "Geen cirkels gedetecteerd - " + std::to_string(totalClockTicks) + " ticks";
        cv::putText(outputImage, noDetectionMsg, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
    }

    return outputImage;
}
