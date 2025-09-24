#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <sstream>

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();

    void log(const std::string& message);
    
private:
    Logger() = default;
    ~Logger() = default;  
};

#endif
