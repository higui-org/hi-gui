#include "higui/parser/himl/line.h"

namespace hi::parser::himl
{
    /*
      class Line 
    (constructors)
          |
          |
         \ /
    */
    Line::Line(size_t spaces_per_tab)
        : total_tabs(0), spaces_per_tab(spaces_per_tab)
    {       
        // If spaces_per_tab is equal to 4, it represents a single tab.
        if (this->spaces_per_tab == 4) 
            this->total_tabs = 1;

        // If spaces_per_tab is greater than 4, calculate total_tabs and 
        // adjust spaces_per_tab to represent remaining spaces.
        else if (this->spaces_per_tab > 4) 
        {
            this->total_tabs = this->spaces_per_tab / 4;
            this->spaces_per_tab %= 4; // Remaining spaces after full tabs
        }
    }

    Line::Line(
        const std::string& m_line,
        size_t spaces_per_tab,
        const std::string& filename,
        int line_num
    )
        :
        total_tabs(0),
        spaces_per_tab(spaces_per_tab)
    {
        size_t spaces = CountLeadingSpaces(m_line);

        // Calculate total_tabs based on the count of spaces and spaces_per_tab
        this->total_tabs = spaces / this->spaces_per_tab;
        size_t remaining_spaces = spaces % this->spaces_per_tab;

        // Adjust type and spaces_per_tab based on remaining spaces
        this->spaces_per_tab = remaining_spaces;
        this->assign(Line::TrimSpaces(m_line.substr(spaces)));
    }


    /*
      class Line
          |
          |
         \ /
    */
     
    size_t Line::CountLeadingSpaces(const std::string& str) noexcept
    {
        size_t space_count = 0;

        for (char ch : str) 
        {
            switch (ch)
            {
            case ' ':
                space_count++; break;
            case '\t':
                space_count += 4; break;
            default:
                return space_count;
            }
        }

        return space_count;
    }

    size_t Line::CountLeadingSpaces() const noexcept
    {
        return Line::CountLeadingSpaces(*this);
    }

    bool Line::StartsWith(const std::string& str) const noexcept 
    {
        return length() >= str.length() &&
            compare(0, str.length(), str) == 0;
    }

    std::string Line::TrimSpaces(const std::string& str) noexcept
    {
        std::string ret{ str };

        size_t start = ret.find_first_not_of(" \t");
        if (start == std::string::npos) {
            // If the string is empty or consists only of spaces or tabs,
            // clear the string using the clear method inherited from std::string
            ret.clear();
            return;
        }

        size_t end = ret.find_last_not_of(" \t");
        if (end != std::string::npos) {
            // Assign the trimmed substring using the assign method from std::string
            ret.assign(ret.substr(start, end - start + 1));
        }
        else {
            // If there is no character other than space or tab,
            // it means the entire string should be cleared.
            ret.clear();
        }
        return ret;
    }

    std::string Line::TrimSpaces() noexcept
    {
        return Line::TrimSpaces(*this);
    }


    /*
          class Line
       (setters getters)
               |
               |
              \ /
    */
    size_t Line::getTabs() const noexcept 
    {
        return this->total_tabs;
    }

    void Line::setTabs(size_t tabs_n) noexcept
    {
        this->total_tabs = tabs_n;
    }

    size_t Line::getSpacesPerTab() const noexcept 
    {
        return this->spaces_per_tab;
    }

    std::string Line::getLine() const noexcept
    {
        return *this;
    }

    void Line::setSpacesPerTab(size_t spaces) noexcept 
    {
        this->spaces_per_tab = spaces;
    }
} // namespace hi::parser::himl