#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <vector>

#include "parser.hpp"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    /**
     * @brief Waits and checks for user input.
     * 
     * @return std::pair<ShapeType, Color> Parsed shape and color from user input.
     */
    std::pair<ShapeType, Color> checkForInput();
    /**
     * @brief Get the Batch Input vector
     * 
     * @param filePath location of the batch input file
     * @return std::vector<std::pair<ShapeType, Color>>&  Reference to the vector containing batch inputs.
     */
    std::vector<std::pair<ShapeType, Color>>& getBatchInput(const std::string& filePath);
    
private:
    Parser parser;

    /**
     * @brief Validates the parsed input.
     * 
     * @param input Pair of ShapeType and Color to validate.
     * @return true If both shape and color are valid.
     * @return false If either shape or color is UNKNOWN.
     */
    bool validateInput(std::pair<ShapeType, Color> input);

private:
    std::vector<std::pair<ShapeType, Color>> batchInputs;
};

#endif