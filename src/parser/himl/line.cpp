#include "higui/parser/himl/line.h"

namespace hi::parser::himl
{
/*
    class Indent 
        |
        |
        \ /
*/
void Indent::DetermineTabsFrom(const std::string& str) noexcept
{
    size_t spaces = Line::CountLeadingSpaces(str);
    tabs = spaces / spaces_per_tab;
}

/*
    class Line
        |
        |
        \ /
*/
Line::Line(const std::string& line, Indent ind) : indent(ind)
{
    this->indent.DetermineTabsFrom(line);

    // Set the string without spaces or tabs
    this->assign(Line::Trim(line));
}

std::string Line::Trim(const std::string& str) noexcept
{
    std::string processed_str{ str };

    size_t start = processed_str.find_first_not_of(" \t");
    if (start == std::string::npos) {
        // If the string is empty or consists only of spaces or tabs,
        // clear the string using the clear method inherited from std::string
        processed_str.clear();
        return processed_str;
    }

    size_t end = processed_str.find_last_not_of(" \t");
    if (end != std::string::npos) {
        // Assign the trimmed substring using the assign method from std::string
        processed_str.assign(processed_str.substr(start, end - start + 1));
    }
    else {
        // If there is no character other than space or tab,
        // it means the entire string should be cleared.
        processed_str.clear();
    }
    return processed_str;
}

size_t Line::CountLeadingSpaces(const std::string& str) noexcept
{
    size_t spaces = 0;

    for (char ch : str)
    {
        switch (ch)
        {
        case ' ':
            spaces++; break;
        case '\t':
            spaces += 4; break;
        default:
            return spaces;
        }
    }

    return spaces;
}

bool Line::StartsWith(const std::string& str) const noexcept
{
    return length() >= str.length() &&
        compare(0, str.length(), str) == 0;
}

} // namespace hi::parser::himl