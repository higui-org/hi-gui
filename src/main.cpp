#include "higui/parser/parser.h"

using namespace hi::parser;

int main() {
    try
    {
        himl::Parser parser("markup.himl");
        parser.getDOM().PrintTree();
    }
    catch (const ParsingException& pe)
    {
        std::cerr << pe.what() << std::endl;
    }
    

    return 0;
}