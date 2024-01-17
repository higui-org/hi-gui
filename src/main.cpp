#include "higui/parser/markup.h"

#include <iostream>

int main()
{
    hi::HIML himl;
    himl.read("markup.himl");
    himl.getTagManager().printTree();
    
    
    return 0;
}
