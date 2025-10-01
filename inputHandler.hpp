#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "parser.hpp"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    std::pair<ShapeType, Color> checkForInput();
private:
    Parser parser;

    bool validateInput(std::pair<ShapeType, Color> input);
    
};

#endif