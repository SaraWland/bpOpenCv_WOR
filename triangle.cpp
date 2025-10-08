#include "triangle.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include "Logger.hpp"

// cppcheck-suppress unusedFunction ; Called in detectShape() in shapeHandler.cpp
cv::Mat Triangle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;
    
    cv::GaussianBlur(inputImage, processableImage, cv::Size(5, 5), 1, 1);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point>> validTriangles;
    std::vector<long long> individualDetectionTimes; // Store individual detection times
    
    // Start timing for detection process
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Start timing for individual triangle validation
        std::chrono::high_resolution_clock::time_point individualStartTime = std::chrono::high_resolution_clock::now();
        
        // Avoid big areas and small noise areas
        double area = cv::contourArea(contours[i]);
        if (area < 100 || area > 15000)
        {
            continue;
        }   
        // Convert contour to polygon for corner checking
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx, epsilon, true);

        // Check if the polygon is a triangle
        if (approx.size() == 3)
        {
            // Circularity check to filter out half-circles
            // 4 * PI * area / (perimeter * perimeter)
            double perimeter = cv::arcLength(contours[i], true);
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);

            // Above a certain value the shape is too circular
            if (circularity > 0.6) {
                continue;
            }
            
            // With finer approximation, half-circles get more vertices because it detects more details
            std::vector<cv::Point> fineApprox;
            double fineEpsilon = 0.015 * cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], fineApprox, fineEpsilon, true);
            
            if (fineApprox.size() > 6) {
                continue;
            }
            
            // End timing for individual triangle validation
            std::chrono::high_resolution_clock::time_point individualEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<long long, std::nano> individualDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(individualEndTime - individualStartTime);
            
            // Convert nanoseconds to actual clock ticks using approximate CPU frequency
            double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
            long long individualClockTicks = static_cast<long long>(individualDuration.count() * cpuFrequencyGHz);
            
            validTriangles.push_back(contours[i]);
            individualDetectionTimes.push_back(individualClockTicks);
        }
    }
    
    // End timing for detection process
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    
    // Convert nanoseconds to actual clock ticks using approximate CPU frequency
    double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
    long long totalClockTicks = static_cast<long long>(duration.count() * cpuFrequencyGHz);

    // Draw detected triangles
    for (size_t i = 0; i < validTriangles.size(); ++i)
    {
        // Safety check
        if (validTriangles[i].empty()) 
        {
            continue;
        }
        
        // Convert contour to polygon for drawing
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(validTriangles[i], true);
        cv::approxPolyDP(validTriangles[i], approx, epsilon, true);
        
        // Calculate center and area for logging
        cv::Moments M = cv::moments(validTriangles[i]);
        if (M.m00 == 0) 
        {
            continue;
        }
        cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
        double area = cv::contourArea(validTriangles[i]);
        
        // Get the minimum enclosing circle for radius estimation
        cv::Point2f circleCenter;
        float radius;
        cv::minEnclosingCircle(validTriangles[i], circleCenter, radius);
        
        // Draw the triangle
        cv::polylines(outputImage, approx, true, cv::Scalar(0, 255, 0), 2);
        cv::circle(outputImage, center, 3, cv::Scalar(0, 0, 255), -1);
        
        // Draw coords, area and individual clock ticks on output image
        // make sure text is within image bounds and does not overlap with the shape
        std::string info = "X,Y(" + std::to_string(center.x) + ", " + std::to_string(center.y) + "), A: " + std::to_string(static_cast<int>(area)) + ", T: " + std::to_string(individualDetectionTimes[i]) + " ticks";
        
        // Calculate text size to ensure it fits
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(info, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseline);
        
        // Start with default position (right and above the center)
        cv::Point textPos(center.x + static_cast<int>(radius) + 10, center.y - 10);
        
        // Check if text goes beyond right edge
        if (textPos.x + textSize.width > outputImage.cols) {
            textPos.x = center.x - static_cast<int>(radius) - textSize.width - 10; // Move to left side
        }
        
        // Check if text goes beyond left edge
        if (textPos.x < 0) {
            textPos.x = 10; // Move to left margin
        }
        
        // Check if text goes above top edge
        if (textPos.y - textSize.height < 0) {
            textPos.y = center.y + static_cast<int>(radius) + textSize.height + 10; // Move below shape
        }
        
        // Check if text goes below bottom edge
        if (textPos.y > outputImage.rows) {
            textPos.y = outputImage.rows - 10; // Move to bottom margin
        }
        
        cv::putText(outputImage, info, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 0), 1);
        
        // Draw contours on black background for visualization
        cv::polylines(contourImage, approx, true, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);

        if (!isInteractive) 
        {
            // Output x, y, area and individual clock ticks to logger
            Logger::getInstance().log("Driehoek gedetecteerd op X,Y(" + std::to_string(center.x) + ", " + std::to_string(center.y) + "), Oppervlakte in pixels: " + std::to_string(static_cast<int>(area)) + ", Ticks: " + std::to_string(individualDetectionTimes[i]) + " ticks\n" );
        }
    }
    
    // Always display detection timing information
    if (validTriangles.size() == 0){
        std::string noDetectionMsg = "Geen driehoeken gedetecteerd - " + std::to_string(totalClockTicks) + " ticks";
        cv::putText(outputImage, noDetectionMsg, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        if (!isInteractive)
        {
            Logger::getInstance().log(noDetectionMsg + "\n");
        }
    }
    
    return outputImage;
}
