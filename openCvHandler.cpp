#include <iostream>

#include "openCvHandler.hpp"
#include "Logger.hpp"


OpenCvHandler::OpenCvHandler(bool interactiveMode)
    : cap(4), isInteractiveMode(interactiveMode), captureAvailable(true), inputHandler(InputHandler()), shapeHandler(ShapeHandler()), colorManager(ColorManager()), shouldStop(false)
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
        cv::moveWindow("outputWindow", 10, 0);
        cv::namedWindow("processedColorWindow");
        cv::moveWindow("processedColorWindow", 10, 675);
        cv::namedWindow("processedShapeWindow");
        cv::moveWindow("processedShapeWindow", 10, 1300);
        cv::namedWindow("resultWindow");
        cv::moveWindow("resultWindow", 1500, 0);

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
        cap >> filterImage;
        cap >> outputImage;
        cap >> contourImage;
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
    cv::imshow("processedColorWindow", filterImage);
    cv::imshow("processedShapeWindow", contourImage);
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

            cv::Mat colorMask = colorManager.getMask(originalImage, parsedInput.second);
            cv::Mat tempContourImage;
            cv::Mat processedImage = shapeHandler.detectShape(colorMask, parsedInput.first, originalImage, tempContourImage);

            // Show the filter image and processed image
            filterImage = colorMask;
            contourImage = tempContourImage;
            outputImage = processedImage;

        } // No sleep because always waiting for input
    });
}