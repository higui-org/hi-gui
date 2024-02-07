#include "higui/parser/himl/section.h"

namespace hi::parser::himl 
{

    /*
      ITERATORS  GETTERS
               |
               |
              \ /
    */

    std::string Section::getName() const 
    {
        return name;
    }

    
    std::vector<Line>::iterator Section::begin() 
    {
        return lines.begin();
    }

    std::vector<Line>::iterator Section::end() 
    {
        return lines.end();
    }
    

} // namespace hi::parser::himl
