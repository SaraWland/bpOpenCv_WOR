#include "openCvHandler.hpp"
#include <iostream>

OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(0), isInteractiveMode(interactiveMode), captureAvailable(true)
{
    if (!cap.isOpened()) {
        captureAvailable = false;
        std::cout << "Camera input not found. Please launch the program in sudo with one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << std::endl;

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


    cap >> originalImage;
    if (originalImage.empty()) { return; }

    // TODO check for input and handle input


    cv::imshow("outputWindow", originalImage);
    cv::waitKey(30);
}