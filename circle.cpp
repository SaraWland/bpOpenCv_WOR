#include "circle.hpp"
#include <iostream>
#include <vector>

Circle::Circle()
{
    this->type = ShapeType::CIRCLE;
}

cv::Mat Circle::findShape(cv::Mat& inputImage, cv::Mat& originalImage)
{
    cv::Mat outputImage = originalImage.clone();
    cv::Mat processableImage;

    cv::GaussianBlur(inputImage, processableImage, cv::Size(3,3), 2, 2);
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(processableImage, circles, cv::HOUGH_GRADIENT, 1, 40, 100, 30, 20, 80);

    for (size_t i = 0; i < circles.size(); ++i)
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(outputImage, center, radius, cv::Scalar(0, 255, 0), 2);
        cv::circle(outputImage, center, 2, cv::Scalar(0, 0, 255), 3);
        
        // Log circle details
        std::cout << "Circle " << i << ": Center = (" << center.x << ", " << center.y << "), Radius = " << radius << std::endl;
    }

    return outputImage;
}
