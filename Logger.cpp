#include "Logger.hpp"
#include <chrono>
#include <iomanip>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {    
    std::cout << message << std::endl;
}



