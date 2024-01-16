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
    void ShowError(
        const std::string& filename, 
        int line_number, 
        int position, 
        const std::string& message, 
        const std::string& line) const;

    int CountIndentation(const std::string& line);
};

} // namespace hi
#endif // HiGUI_HIML_PARSER_H