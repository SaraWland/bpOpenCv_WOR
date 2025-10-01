#include <iostream>

#include "openCvHandler.hpp"
#include "Logger.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (argc < 2)
    {
        Logger::getInstance().log(
            "\nProgram arguments not found. Please launch the program in one of the following modes:\n\n"
            "------------------------------------\n"
            "| Mode        | Program argument   |\n"
            "|-------------|--------------------|\n"
            "| Interactive | sudo ./bpOpenCV -i |\n"
            "| Batch       | sudo ./bpOpenCV -b |\n"
            "------------------------------------\n\n"
        );
        return -1;
    }

    if (std::string(argv[1]) == "-i")
    {
        // Interactive mode
    } else if (std::string(argv[1]) == "-b")
    {
        // Batch mode
    } else {
        Logger::getInstance().log(
            "\nProgram argument not recognized. Please launch the program in one of the following modes:\n\n"
            "------------------------------------\n"
            "| Mode        | Program argument   |\n"
            "|-------------|--------------------|\n"
            "| Interactive | sudo ./bpOpenCV -i |\n"
            "| Batch       | sudo ./bpOpenCV -b |\n"
            "------------------------------------\n\n"
        );
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