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
    Logger::getInstance().log("\nPlease provide input:\n > ");
    std::getline(std::cin, userInput);

    std::pair<ShapeType, Color> parsedData = parser.parseInput(userInput);

    if (!validateInput(parsedData)) 
    {
        Logger::getInstance().log(
            "\nInput not recognized. Please provide input in the following format: <shape> <color> "
            "(e.g. 'cirkel roze' or 'halve cirkel groen')\n\n"
            "-------------------------------\n"
            "| Shape types: | Colors:      |\n"
            "|--------------|--------------|\n"
            "| cirkel       | roze         |\n"
            "| rechthoek    | groen        |\n"
            "| driehoek     | geel         |\n"
            "| vierkant     | oranje       |\n"
            "| halve cirkel |              |\n"
            "-------------------------------\n\n"
        );
        
        return;
    }

    // TODO handle input
    Logger::getInstance().log("\nValid input received.\n");
}

bool InputHandler::validateInput(std::pair<ShapeType, Color> input) 
{
    return (input.first != ShapeType::UNKNOWN && input.second != Color::UNKNOWN);
}