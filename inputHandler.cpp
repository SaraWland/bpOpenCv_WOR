#include <iostream>
#include <string>
#include <utility>

#include "inputHandler.hpp"
#include "Logger.hpp"

InputHandler::InputHandler()
{
    parser = Parser();
}

std::pair<ShapeType, Color> InputHandler::checkForInput() 
{
    std::string userInput;
    Logger::getInstance().log("\nPlease provide input:\n ~> ");
    std::getline(std::cin, userInput);

    std::pair<ShapeType, Color> parsedData = parser.parseInput(userInput);

    if (parsedData.first == ShapeType::EXIT) {
        Logger::getInstance().log("\nExiting program...\n\n");
        return {ShapeType::EXIT, Color::UNKNOWN};
    }

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

        return {ShapeType::UNKNOWN, Color::UNKNOWN};
    }

    
    Logger::getInstance().log("\nValid input received.\n");
    return parsedData;

}

bool InputHandler::validateInput(std::pair<ShapeType, Color> input) 
{
    return (input.first != ShapeType::UNKNOWN && input.second != Color::UNKNOWN);
}