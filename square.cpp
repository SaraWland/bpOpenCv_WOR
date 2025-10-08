#include "square.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "Logger.hpp"

Square::Square()
{
    this->type = ShapeType::SQUARE;
}

cv::Mat Square::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage, bool isInteractive)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;
    
    cv::GaussianBlur(inputImage, processableImage, cv::Size(5, 5), 1, 1);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point>> validSquares;
    std::vector<long long> individualDetectionTimes; // Store individual detection times
    
    // Start timing for detection process
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Start timing for individual square validation
        std::chrono::high_resolution_clock::time_point individualStartTime = std::chrono::high_resolution_clock::now();
        
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
                // End timing for individual square validation
                std::chrono::high_resolution_clock::time_point individualEndTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<long long, std::nano> individualDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(individualEndTime - individualStartTime);
                
                // Convert nanoseconds to actual clock ticks using approximate CPU frequency
                double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
                long long individualClockTicks = static_cast<long long>(individualDuration.count() * cpuFrequencyGHz);
                
                validSquares.push_back(contours[i]);
                individualDetectionTimes.push_back(individualClockTicks);
            }
        }
    }
    
    // End timing for detection process
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    
    // Convert nanoseconds to actual clock ticks using approximate CPU frequency
    double cpuFrequencyGHz = 4.0; // Approximate CPU frequency in GHz
    long long totalClockTicks = static_cast<long long>(duration.count() * cpuFrequencyGHz);

    // Draw detected squares
    for (size_t i = 0; i < validSquares.size(); ++i)
    {
        // Safety check
        if (validSquares[i].empty()) 
        {
            continue;
        }
        
        // Convert contour to polygon for drawing
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(validSquares[i], true);
        cv::approxPolyDP(validSquares[i], approx, epsilon, true);
        
        // Calculate center and area for logging
        cv::Moments M = cv::moments(validSquares[i]);
        if (M.m00 == 0) 
        {
            continue;
        }
        cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
        double area = cv::contourArea(validSquares[i]);
        
        // Calculate side lengths for radius estimation
        std::vector<double> sideLengths;
        for (int j = 0; j < 4; j++)
        {
            cv::Point p1 = approx[j];
            cv::Point p2 = approx[(j + 1) % 4];
            double length = cv::norm(p1 - p2);
            sideLengths.push_back(length);
        }
        double avgSide = (sideLengths[0] + sideLengths[1] + sideLengths[2] + sideLengths[3]) / 4.0;
        double radius = avgSide / 2.0;
        
        // Draw the square
        cv::polylines(outputImage, approx, true, cv::Scalar(255, 0, 0), 2);
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
            Logger::getInstance().log("Rechthoek gedetecteerd op X,Y(" + std::to_string(center.x) + ", " + std::to_string(center.y) + "), Oppervlakte in pixels: " + std::to_string(static_cast<int>(area)) + ", Ticks: " + std::to_string(individualDetectionTimes[i]) + " ticks\n" );
        }
    }
    
    // Always display detection timing information
    if (validSquares.size() == 0){
        std::string noDetectionMsg = "Geen vierkanten gedetecteerd - " + std::to_string(totalClockTicks) + " ticks";
        cv::putText(outputImage, noDetectionMsg, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        if (!isInteractive)
        {
            Logger::getInstance().log(noDetectionMsg + "\n");
        }
    }

    
    return outputImage;
}