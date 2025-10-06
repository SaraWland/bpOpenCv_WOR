#include "halfCircle.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Logger.hpp"

HalfCircle::HalfCircle()
{
    this->type = ShapeType::HALF_CIRCLE;
}

cv::Mat HalfCircle::findShape(cv::Mat& inputImage, cv::Mat& originalImage, cv::Mat& contourImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;

    cv::GaussianBlur(inputImage, processableImage, cv::Size(5,5), 1, 1);
    
    // Reduc noise
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(processableImage, processableImage, cv::MORPH_CLOSE, kernel);
    
    // Find all contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(processableImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point>> validHalfCircles;
    
    for (size_t i = 0; i < contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);

        // Avoid big areas and small noise areas
        if (area < 200 || area > 15000) 
        {
            continue; 
        }

        cv::Moments M = cv::moments(contours[i]);
        // .m00 is the area of the contour
        if (M.m00 == 0) 
        {
            continue;
        }

        // Convert contour to polygon to with fine approximation, half-circles get more vertices because it detects more details
        std::vector<cv::Point> approx;
        double epsilon = 0.02 * cv::arcLength(contours[i], true); 
        cv::approxPolyDP(contours[i], approx, epsilon, true);
        
        if (approx.size() < 5) 
        {
            continue;
        }

        // Circularity check to filter out half-circles
        // 4 * PI * area / (perimeter * perimeter)
        double perimeter = cv::arcLength(contours[i], true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);
        
        // Get bounding rectangle and rotated rectangle
        cv::Rect boundingRectangle = cv::boundingRect(contours[i]);
        cv::RotatedRect rotatedRect = cv::minAreaRect(contours[i]);
        
        // Calculate values for checking
        double aspectRatio = static_cast<double>(boundingRectangle.width) / boundingRectangle.height;
        double rectArea = boundingRectangle.width * boundingRectangle.height;
        double extent = area / rectArea;
        
        // If shape is convex, calculate the convexity
        std::vector<cv::Point> hull;
        cv::convexHull(contours[i], hull);
        double hullArea = cv::contourArea(hull);
        double convexity = 0;
        if (hullArea > 0)
        {
            convexity = area / hullArea;
        }

        bool isHalfCircle = false;

        // Get the enclosing circle. Area should be half of that. Defined with a ratio
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[i], center, radius);
        double circleArea = CV_PI * radius * radius;
        double areaRatio = area / (circleArea * 0.5); // Compare to half-circle area
        
        // Look for a straight edge
        bool hasLongStraightEdge = false;
        
        if (contours[i].size() >= 8) {  
            // Check for sequences of points that form approximately straight lines
            for (size_t j = 0; j < contours[i].size() - 4; ++j) { 
                // Calculate line lengthfrom point j to j+4
                cv::Point p1 = contours[i][j];
                cv::Point p2 = contours[i][j + 4];  
                double lineLength = cv::norm(p1 - p2);
                
                // Count points close to this line
                int pointsOnLine = 0;
                for (size_t k = j; k <= j + 4; ++k) { 
                    // Comparing point
                    cv::Point p = contours[i][k];

                    // Calculate distance from point to line by:
                    // Calculating the area of the triangle formed by p1, p2 and p.
                    // Divide that area by the length of p1 to p2.
                    double distance = std::abs((p2.y - p1.y) * p.x - (p2.x - p1.x) * p.y + p2.x * p1.y - p2.y * p1.x) / 
                                 std::sqrt((p2.y - p1.y) * (p2.y - p1.y) + (p2.x - p1.x) * (p2.x - p1.x));
                    
                    // Below a certain distance it's considered on the line
                    if (distance < 4.0) pointsOnLine++;
                }
                
                // If enough points are on the line and the line is longer than the radius it's considered a straight edge
                // Multiplying the radius creates tolerance
                if (pointsOnLine >= 3 && lineLength > radius * 0.4) {
                    hasLongStraightEdge = true;
                    break;
                }
            }
        }
        
        // If no straight line found check if one edge of the minimum rectangle is significantly larger than the adjacent one
        if (!hasLongStraightEdge && contours[i].size() >= 6) {

            cv::RotatedRect minimumRectangle = cv::minAreaRect(contours[i]);
            cv::Point2f vertices[4];
            minimumRectangle.points(vertices);
            
            double side1 = cv::norm(vertices[1] - vertices[0]);
            double side2 = cv::norm(vertices[2] - vertices[1]);
            double ratio = std::max(side1, side2) / std::min(side1, side2);
            
            if (ratio > 1.4) {
                hasLongStraightEdge = true;
            }
        }

        // Use all calculated values for checking if it's a half-circle
        if (circularity > 0.67 && circularity < 0.8 &&
            convexity > 0.82 &&
            aspectRatio > 0.5 && aspectRatio < 2.2 &&
            extent > 0.35 &&
            areaRatio > 0.6 && areaRatio < 1.5 &&
            hasLongStraightEdge)
        {
            // Finer approximation to filter out triangles
            std::vector<cv::Point> fineApprox;
            double fineEpsilon = 0.01 * cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], fineApprox, fineEpsilon, true);
            
            // Half circles get more sides with finer approximation
            // If still under 4 sides it's a triangle
            if (fineApprox.size() <= 4) {
                continue;
            }
            
            validHalfCircles.push_back(contours[i]);
            isHalfCircle = true;
        }
    }
    
    // Draw detected half-circles
    for (size_t i = 0; i < validHalfCircles.size(); ++i)
    {
        // Safety check
        if (validHalfCircles[i].empty()) 
        {
            continue;
        }
        
        // Calculate center for logging
        cv::Moments M = cv::moments(validHalfCircles[i]);
        if (M.m00 == 0) 
        {
            continue;
        }
        cv::Point center(M.m10 / M.m00, M.m01 / M.m00);


        double area = cv::contourArea(validHalfCircles[i]);
        // Get the minimum enclosing circle for radius
        cv::Point2f circleCenter;
        float radius;
        cv::minEnclosingCircle(validHalfCircles[i], circleCenter, radius);
        
        // Draw the actual half-circle contour
        cv::polylines(outputImage, validHalfCircles[i], true, cv::Scalar(255, 165, 0), 2);
        
        // Draw on contour image
        cv::polylines(contourImage, validHalfCircles[i], true, cv::Scalar(255, 255, 255), 2);
        cv::circle(contourImage, center, 3, cv::Scalar(0, 255, 0), -1);
    
        Logger::getInstance().log("Half-circle detected at (" + std::to_string(center.x) + ", " + std::to_string(center.y) + ") with area " + std::to_string(static_cast<int>(area)) + ") and radius " + std::to_string(static_cast<int>(radius)) );
    }

    return outputImage;
}