#include "higui/parser/himl/himl.h"

#include <iostream>

using namespace hi::parser;

int main() {
    try
    {
        himl::HIML h("markup.himl");
        himl::HIML h2("view.himl");
    }
    catch (ParsingException pe)
    {
        std::cerr << pe.what();
    }

    return 0;
}