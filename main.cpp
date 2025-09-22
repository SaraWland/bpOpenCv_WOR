#include <iostream>

#include "openCvHandler.hpp"

using namespace cv;
using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (argc < 2)
    {
        cout << "Program arguments not found. Please launch the program in one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << endl;
        return -1;
    }

    if (string(argv[1]) == "-i")
    {
        // Interactive mode
    } else if (string(argv[1]) == "-b")
    {
        // Batch mode
    } else {
        cout << "Program argument not recognized. Please launch the program in one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << endl;
        return -1;
    }

    OpenCvHandler handler(string(argv[1]) == "-i");
    while(true) 
    {
        handler.updateImage();
    }

    return 0;
}