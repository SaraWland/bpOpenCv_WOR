#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <fstream>

#include "parser.hpp"
#include "Logger.hpp"

std::pair<ShapeType, Color> Parser::parseInput(const std::string& input)
{
    std::vector<std::string> tokens;
    std::string currentToken;

    for (char character : input) {
        if (character == ' ') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            } else {
                continue; 
            }
        } else {
            currentToken += character;
        }
    }
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    ShapeType shape = ShapeType::UNKNOWN;
    Color color = Color::UNKNOWN;

    if (tokens.empty()) {
        return {ShapeType::UNKNOWN, Color::UNKNOWN};
    } else if (tokens.size() > 3) {
        return {ShapeType::UNKNOWN, Color::UNKNOWN};
    } else if (tokens.size() < 2)
    {
        if (tokens[0].compare("exit") == 0) 
        {
            return {ShapeType::EXIT, Color::UNKNOWN};
        } else {
            return {ShapeType::UNKNOWN, Color::UNKNOWN};
        }
    } else if (tokens.size() == 3) {
        std::vector<std::string> tempTokens;
        tempTokens.push_back(tokens[0] + " " + tokens[1]);
        tempTokens.push_back(tokens[2]);
        tokens = tempTokens;
    } 

    if (tokens[0].compare("cirkel") == 0) {
        shape = ShapeType::CIRCLE;
    } else if (tokens[0].compare("rechthoek") == 0) {
        shape = ShapeType::RECTANGLE;
    } else if (tokens[0].compare("driehoek") == 0) {
        shape = ShapeType::TRIANGLE;
    } else if (tokens[0].compare("vierkant") == 0) {
        shape = ShapeType::SQUARE;
    } else if (tokens[0].compare("halve cirkel") == 0) {
        shape = ShapeType::HALF_CIRCLE;
    } else {
        shape = ShapeType::UNKNOWN;
    }    

    if (tokens[1].compare("roze") == 0) {
        color = Color::PINK;
    } else if (tokens[1].compare("groen") == 0) {
        color = Color::GREEN;
    } else if (tokens[1].compare("geel") == 0) {
        color = Color::YELLOW;
    } else if (tokens[1].compare("oranje") == 0) {
        color = Color::ORANGE;
    } else {
        color = Color::UNKNOWN;
    }

    return {shape, color};
}

// cppcheck-suppress unusedFunction ; Used in inputHandler.cpp getBatchInput() for batch file parsing
void Parser::parseBatchInput(const std::string& batchFilePath, std::vector<std::pair<ShapeType, Color>>& batchInputs)
{
    std::ifstream file(batchFilePath);
    if (!file.is_open()) {
        Logger::getInstance().log("Error: Could not open batch input file: " + batchFilePath + "\n");
        return;
    }

    int i = 0;

    std::string line;
    while (std::getline(file, line)) {
        i += 1;

        // Ignore empty lines
        if (line.empty()) {
            continue;
        }

        // If # then ignore rest of line
        for (size_t j = 0; j < line.size(); ++j) {
            if (line[j] == '#') {
                line.resize(j);
                break;
            }
        }
        if (line.empty())
        {
            continue;
        }

        // Parse lines with parseLine
        std::pair<ShapeType, Color> parsed = parseInput(line);
        // Store in batchInputs vector if valid input
        if (parsed.first == ShapeType::UNKNOWN || parsed.second == Color::UNKNOWN || parsed.first == ShapeType::EXIT) 
        {
            Logger::getInstance().log("Warning: Invalid input at line " + std::to_string(i) + ": '" + line + "'. Skipping this entry.\n");
            continue;
        }
        batchInputs.push_back(parsed);
    }

    file.close();
    Logger::getInstance().log("Batch input file parsed successfully. Total entries: " + std::to_string(batchInputs.size()) + "\n");
}