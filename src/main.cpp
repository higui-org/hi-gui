#include "higui/parser/himl/himl.h"

#include <iostream>

using namespace hi;
using namespace parser;
using namespace parser::himl;

int main() {
    try {
        // Creating a base Indent object with a specific number of spaces per tab
        Indent base(2); // Assumes 2 spaces equal one tab
        // "base" would have 0 tabs

        // Creating another Indent object using a string, base indent, 
        // (filename, and line number provides info for ParsingException, they are not mandatory)
        // This example uses a line with 4 leading spaces
        Indent custom("    Line with leading spaces", base, "example_file.himl", 5);
        // "custom" would have 2 tabs of total, 'cause "base" declares 2 spaces as 1 tab.

        Indent custom_one_tab("    Line with leading spaces", Indent(), "example_file.himl", 5);
        // "custom_one_tab" would have 1 tab of total, 
        // 'cause default constructor for param "base_indent" initialized 4 spaces for 1 tab.
    }
    catch (const ParsingException& e) {
        std::cerr << "Parsing exception: " << e.what() << std::endl;
    }

    return 0;
}
