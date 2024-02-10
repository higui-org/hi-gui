#include "higui/parser/parser.h"

using namespace hi::parser;

int main() {
    himl::Parser parser("markup.himl");
    parser.getDOM().PrintTree();

    

    return 0;
}