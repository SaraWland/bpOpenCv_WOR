#include <iostream>

#include "openCvHandler.hpp"
#include "shapeHandler.hpp"
#include "Logger.hpp"


OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(0), isInteractiveMode(interactiveMode), captureAvailable(true), inputHandler(InputHandler()), shouldStop(false)
{
    if (!cap.isOpened()) {
        captureAvailable = false;
        Logger::getInstance().log(
            "\nCamera input not found. Please launch the program in sudo with one of the following modes:\n\n"
            "------------------------------------\n"
            "| Mode        | Program argument   |\n"
            "|-------------|--------------------|\n"
            "| Interactive | sudo ./bpOpenCV -i |\n"
            "| Batch       | sudo ./bpOpenCV -b |\n"
            "------------------------------------\n\n"
        );
        return;
    }

    cv::namedWindow("outputWindow");
    if (isInteractiveMode) {
        setupInputThread();
    }
    Logger::getInstance().log(
        "\nWelcome to the interactive mode of the program. You can now provide input via the terminal.\n"
        "Please provide input in the following format: <shape> <color> "
        "(e.g. 'cirkel roze' or 'halve cirkel groen')\n\n"
        "-------------------------------\n"
        "| Shape types: | Colors:      |\n"
        "|--------------|--------------|\n"
        "| cirkel       | roze         |\n"
        "| rechthoek    | groen        |\n"
        "| driehoek     | geel         |\n"
        "| vierkant     | oranje       |\n"
        "| halve cirkel |              |\n"
        "-------------------------------\n\n"
    );
    updateImage();
}

OpenCvHandler::~OpenCvHandler()
{
    shouldStop = true;
    if (inputThread.joinable()) {
        inputThread.join();
    }
    cap.release();
    cv::destroyWindow("outputWindow");
}

void OpenCvHandler::updateImage()
{


    cap >> originalImage;
    if (originalImage.empty()) { return; }

    cv::imshow("outputWindow", originalImage);
    cv::waitKey(30);
}

void OpenCvHandler::setupInputThread() {
    inputThread = std::thread([this]() {
        while (!shouldStop) {
            inputHandler.checkForInput();
        }
    });
}