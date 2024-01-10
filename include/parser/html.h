#ifndef HiGUI_HIML_PARSER_H
#define HiGUI_HIML_PARSER_H

#include <fstream>
#include <regex>
#include <stack>
#include <iostream>

#include "parser.h"

namespace hi 
{

class HIML : public Parser 
{
public:
    // Reads and parses the HIML file
    bool read(const std::string& filename) noexcept override;

private:
    int CountIndentation(const std::string& line, int line_number);
};

} // namespace hi
#endif // HiGUI_HIML_PARSER_H