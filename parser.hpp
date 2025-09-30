#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <sstream>

#include "enums.hpp"

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    std::pair<ShapeType, Color> parseInput(const std::string& input);
private:

};

#endif