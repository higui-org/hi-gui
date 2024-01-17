#ifndef HiGUI_HIML_PARSER_H
#define HiGUI_HIML_PARSER_H

#include <fstream>
#include <regex>
#include <stack>
#include <iostream>
#include <stdexcept>

#include "higui/parser/parser.h"

namespace hi 
{
class HIML : public Parser 
{
public:
    // Reads and parses the HIML file
    void read(const std::string& filename) override;
};

} // namespace hi
#endif // HiGUI_HIML_PARSER_H