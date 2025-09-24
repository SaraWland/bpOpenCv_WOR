#ifndef OPENCV_HANDLER_HPP
#define OPENCV_HANDLER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class OpenCvHandler {
public:
    explicit OpenCvHandler(bool interactiveMode);//TODO done because only bool in constructor. if changed changed back from explicit
    ~OpenCvHandler();

    void updateImage();

    bool getCaptureAvailable() const { return captureAvailable; }
private:
    cv::Mat originalImage;
    cv::Mat outputImage;
    cv::VideoCapture cap;
    bool isInteractiveMode;
    bool captureAvailable;
};

#endif