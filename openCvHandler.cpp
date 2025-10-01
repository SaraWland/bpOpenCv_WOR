#include <iostream>

#include "openCvHandler.hpp"
#include "Logger.hpp"


OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(0), isInteractiveMode(interactiveMode), captureAvailable(true), inputHandler(InputHandler()), shapeHandler(ShapeHandler()), colorManager(ColorManager()), shouldStop(false)
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

    if (isInteractiveMode) {
        cv::namedWindow("outputWindow");
        cv::moveWindow("outputWindow", 100, 100);
        cv::namedWindow("resultWindow");
        cv::moveWindow("resultWindow", 100, 800);

        setupInputThread();
    
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
        cap >> outputImage;
        updateImage();
    }

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
    cv::imshow("resultWindow", outputImage);
    cv::waitKey(30);


}

void OpenCvHandler::setupInputThread() {
    inputThread = std::thread([this]() {
        while (!shouldStop) {

            std::pair<ShapeType, Color> parsedInput = inputHandler.checkForInput();

            if (parsedInput.first == ShapeType::EXIT) {
                shouldStop = true;  
                break;             
            }

            if (parsedInput.first == ShapeType::UNKNOWN || parsedInput.second == Color::UNKNOWN) {
                continue;
            }

            // Process valid input
            Logger::getInstance().log("Processing input...");
            // TODO: Add image processing

            cv::Mat colorMask = colorManager.getMask(originalImage, parsedInput.second);
            cv::Mat processedImage = shapeHandler.detectShape(colorMask, parsedInput.first);

            // Convert mask to color image for overlay
            cv::Mat colorProcessed;
            cv::cvtColor(colorMask, colorProcessed, cv::COLOR_GRAY2BGR);
            
            // Blend with original image to show color mask overlay
            cv::Mat result;
            cv::addWeighted(originalImage, 0.7, colorProcessed, 0.3, 0, result);

            outputImage = result;

        } // No sleep because always waiting for input
    });
}