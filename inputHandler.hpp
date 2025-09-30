#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "parser.hpp"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    void checkForInput();
private:
    Parser parser;

    bool validateInput(std::pair<ShapeType, Color> input);
    
};

#endif