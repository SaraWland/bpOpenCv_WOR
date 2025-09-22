#include "openCvHandler.hpp"
#include <iostream>

OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(0), isInteractiveMode(interactiveMode)
{
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video capture." << std::endl;
        return;
    }
    cv::namedWindow("outputWindow");
    updateImage();
}

OpenCvHandler::~OpenCvHandler()
{
    cap.release();
    cv::destroyWindow("outputWindow");
}

void OpenCvHandler::updateImage()
{
    // outputImage = originalImage.clone();
    cap >> originalImage;
    if (originalImage.empty()) { return; }

    cv::imshow("outputWindow", originalImage);
    cv::waitKey(30);
}