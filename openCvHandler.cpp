#include <iostream>

#include "openCvHandler.hpp"
#include "shapeHandler.hpp"
#include "Logger.hpp"


OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(0), isInteractiveMode(interactiveMode), captureAvailable(true), inputHandler(InputHandler())
{
    if (!cap.isOpened()) {
        captureAvailable = false;
        // std::cout << "Camera input not found. Please launch the program in sudo with one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << std::endl;
        Logger::getInstance().log("Camera input not found. Please launch the program in sudo with one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b");
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

    // TODO BLOKKEERT DE CAMERA. INPUT MOET WSS OP EEN LOSSE THREAD
    // if (isInteractiveMode) {
    //     inputHandler.checkForInput();
    // }


    cv::imshow("outputWindow", originalImage);
    cv::waitKey(30);
}