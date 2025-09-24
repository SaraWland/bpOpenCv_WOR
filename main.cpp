#include <iostream>

#include "openCvHandler.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (argc < 2)
    {
        std::cout << "Program arguments not found. Please launch the program in one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << std::endl;
        return -1;
    }

    if (std::string(argv[1]) == "-i")
    {
        // Interactive mode
    } else if (std::string(argv[1]) == "-b")
    {
        // Batch mode
    } else {
        std::cout << "Program argument not recognized. Please launch the program in one of the following modes:\nMode        | Program argument\n------------|------------------------\nInteractive | sudo ./bpOpenCV -i\nBatch       | sudo ./bpOpenCV -b" << std::endl;
        return -1;
    }

    OpenCvHandler handler(std::string(argv[1]) == "-i");
    //stops program if no camera found
    if (!handler.getCaptureAvailable()) {
        exit(-1);
    }

    while(true) 
    {
        handler.updateImage();
    }

    return 0;
}