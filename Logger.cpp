#include "Logger.hpp"
#include <chrono>
#include <iomanip>

// cppcheck-suppress unusedFunction ; Singleton pattern - used throughout the code for logging
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// cppcheck-suppress unusedFunction ; Used throughout the code via getInstance() for message logging
void Logger::log(const std::string& message) {    
    std::cout << message;
}



