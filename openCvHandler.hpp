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
    /**
     * @brief Construct a new Open Cv Handler object
     * @param interactiveMode
     * @details If true shows OpenCV windows for interactive mode, if false runs in batch mode.
     *          Also responsible for setting up the initial state of the program.
     */
    explicit OpenCvHandler(bool interactiveMode);
    /**
     * @brief Destroy the Open Cv Handler object
     * @details Cleans up resources and ensures proper shutdown of threads and OpenCV windows.
     */
    ~OpenCvHandler();

    // Delete copy constructor and assignment operator to prevent copying 
    OpenCvHandler(const OpenCvHandler&) = delete;
    OpenCvHandler& operator=(const OpenCvHandler&) = delete;

    /**
     * @brief Updates the displayed images in the OpenCV windows.
     * @details Captures a webcam frame and updates the output.
     *          Updates the filter image, contour image, and output image based on the image mats of OpenCvHandler.
     */
    void updateImage();

    /**
     * @brief Sets up the input thread for processing user commands.
     * @details This function initializes a separate thread that listens for user input and processes it accordingly.
     *          The thread runs in a loop until the shouldStop flag is set to true.
     */
    void setupInputThread();
    /**
     * @brief Processes the batch mode input.
     * @details This function reads a batch input file and processes each entry.
     */
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