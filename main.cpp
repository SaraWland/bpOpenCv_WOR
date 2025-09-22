#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImage;
Mat outputImage;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (argc < 2)
    {
        cout << "Program arguments not found. Please launch the program in one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << endl;
        return -1;
    }

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