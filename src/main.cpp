#include "parser/html.h"

#include <iostream>

int main()
{
    hi::HIML himl;
    himl.read("html.html");
    himl.getTagManager().printTree();
    
    
    return 0;
}
