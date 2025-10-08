#ifndef OPENCV_HANDLER_HPP
#define OPENCV_HANDLER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>
#include <atomic>

#include "inputHandler.hpp"
#include "shapeHandler.hpp"
#include "colorManager.hpp"

class OpenCvHandler {
public:
    explicit OpenCvHandler(bool interactiveMode);//TODO done because only bool in constructor. if changed changed back from explicit
    ~OpenCvHandler();

    // Delete copy constructor and assignment operator to prevent copying 
    OpenCvHandler(const OpenCvHandler&) = delete;
    OpenCvHandler& operator=(const OpenCvHandler&) = delete;

    void updateImage();

    void setupInputThread();
    void processBatchMode();

    // Getters for private members
    // cppcheck-suppress unusedFunction ; Used in main.cpp for camera availability check
    bool getCaptureAvailable() const { return captureAvailable; } 
    
    // cppcheck-suppress unusedFunction ; Used in main.cpp for program exit control
    bool getShouldStop() const { return shouldStop; }
private:
    cv::Mat originalImage;
    cv::Mat filterImage;
    cv::Mat outputImage;
    cv::Mat contourImage;
    cv::VideoCapture cap;
    bool isInteractiveMode;
    bool captureAvailable;
    InputHandler inputHandler;
    ShapeHandler shapeHandler;
    ColorManager colorManager;

    std::thread inputThread;
    std::atomic<bool> shouldStop;

    std::string batchFilePath;
};

#endif