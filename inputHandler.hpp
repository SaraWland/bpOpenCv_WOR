#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <vector>

#include "parser.hpp"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    std::pair<ShapeType, Color> checkForInput();
    std::vector<std::pair<ShapeType, Color>>& getBatchInput(const std::string& filePath);
    
private:
    Parser parser;

    bool validateInput(std::pair<ShapeType, Color> input);

private:
    std::vector<std::pair<ShapeType, Color>> batchInputs;
};

#endif