#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImage;
Mat outputImage;

int main([[maybe_unused]] int arc, [[maybe_unused]] char** argv)
{

    VideoCapture cap(0);
    namedWindow("outputWindow");
    while(true) {
        cap >> originalImage;
        if (originalImage.empty()) break;
        
        outputImage = originalImage.clone();
        imshow("outputWindow", outputImage);
        if(waitKey(30) >= 0) break;
    }
    cap.release();
    destroyWindow("outputWindow");
    return 0;
}