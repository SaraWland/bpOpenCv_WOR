#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <sstream>

#include "enums.hpp"

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    /**
     * @brief Parses a single line of input into shape and color.
     * 
     * @param input The input string to parse.
     * @return std::pair<ShapeType, Color> Parsed shape and color.
     */
    std::pair<ShapeType, Color> parseInput(const std::string& input);
    /**
     * @brief Parses a batch input file into a vector of shape and color pairs.
     * 
     * @param batchFilePath The path to the batch input file.
     * @param batchInputs Reference to the vector where parsed inputs will be stored.
     */
    void parseBatchInput(const std::string& batchFilePath, std::vector<std::pair<ShapeType, Color>>& batchInputs);
private:

};

#endif