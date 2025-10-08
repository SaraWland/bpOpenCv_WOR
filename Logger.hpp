#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <sstream>

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    /**
     * @brief Get the Instance of the Logger
     * 
     * @return Logger& Reference to the singleton Logger instance.
     */
    static Logger& getInstance();

    /**
    * @brief Logs a message to the standard output.
    * 
    * @param message The message to log.
    */
    void log(const std::string& message);
    
private:
    Logger() = default;
    ~Logger() = default;  
};

#endif
