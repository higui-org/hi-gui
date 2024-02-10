#include "higui/parser/himl/section.h"

namespace hi::parser::himl 
{

/*
        iter  set get
            |
            |
            \ /
*/

void Section::setIndent(Indent ind) noexcept
{
    indent.setSpacesPerTab(ind.getSpacesPerTab());
    for (auto& line : lines)  // iterate all lines
    {
        line.indent.setSpacesPerTab(ind.getSpacesPerTab()); // set spaces per tab for each line
    }
}

// static method for extracting the section name from the line
std::string Section::ExtractName(const std::string& raw_line, const std::string& filename, int line_num)
{
    // Assuming the format is '[name]', extract the name part
    size_t start_bracket = raw_line.find('[');
    size_t end_bracket = raw_line.find(']', start_bracket);

    if (start_bracket == std::string::npos || end_bracket == std::string::npos)
        throw ParsingException("Syntax for section name is incorrect", raw_line, filename, line_num);

    std::string name = raw_line.substr(start_bracket + 1, end_bracket - start_bracket - 1);

    name = Line::Trim(name);

    if (name.empty())
        throw ParsingException("Section name not found", raw_line, filename, line_num);

    return name;
}

std::string Section::getName() const noexcept
{
    return name;
}

    
std::vector<Line>::iterator Section::begin() noexcept
{
    return lines.begin();
}

std::vector<Line>::iterator Section::end() noexcept
{
    return lines.end();
}

std::vector<Line>::const_iterator Section::begin() const noexcept
{
    return lines.cbegin();
}

std::vector<Line>::const_iterator Section::end() const noexcept
{
    return lines.cend();
}
    

} // namespace hi::parser::himl
