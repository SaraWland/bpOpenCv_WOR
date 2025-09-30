#include <iostream>
#include <string>
#include <utility>

#include "inputHandler.hpp"
#include "Logger.hpp"

InputHandler::InputHandler()
{
    parser = Parser();
}

void InputHandler::checkForInput() 
{
    std::string userInput;
    std::getline(std::cin, userInput);

    std::pair<ShapeType, Color> parsedData = parser.parseInput(userInput);

    if (!validateInput(parsedData)) 
    {
        Logger::getInstance().log("Input not recognized. Please provide input in the following format: <shape> <color> (e.g. 'cirkel roze' or 'halve cirkel groen')");
        return;
    }

    // TODO handle input
    Logger::getInstance().log("Valid input received.");
}

bool InputHandler::validateInput(std::pair<ShapeType, Color> input) 
{
    return (input.first != ShapeType::UNKNOWN && input.second != Color::UNKNOWN);
}