#include "higui/parser/himl/indent.h"

namespace hi::parser::himl
{
    /*
        Indent 
        (various methods 
        without setters/getters)
          |
          |
         \ /
    */
    Indent::Indent(size_t spaces_per_tab)
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

    Indent::Indent(
        const std::string& line_with_indents, 
        Indent base_indent,
        const std::string& filename,
        int line_num
            )
        :
        total_tabs(0), 
        spaces_per_tab(base_indent.getSpacesPerTab()) // Use base_indent's spaces_per_tab
    {
        size_t space_count = 0;

        // Iterate through each character to count leading spaces
        for (char ch : line_with_indents) {
            if (ch == ' ') {
                space_count++; // Count space characters
            }
            else {
                break; // Stop if a non-space character is encountered
            }
        }

        // Calculate total_tabs based on the count of spaces and spaces_per_tab
        this->total_tabs = space_count / this->spaces_per_tab;
        size_t remaining_spaces = space_count % this->spaces_per_tab;

        // Adjust type and spaces_per_tab based on remaining spaces
        if (remaining_spaces > 0)
            this->spaces_per_tab = remaining_spaces;

        // Throw an exception if no leading spaces or tab found
        if (space_count == 0) {
            throw ParsingException(
                "The line: \"" + line_with_indents
                + "\", does not start with spaces or a tab.",
                filename,
                line_num
            );
        }
    }


    /*
        SETTERS GETTERS
               |
               |
              \ /
    */
    size_t Indent::getTabs() const {
        return total_tabs;
    }

    size_t Indent::getSpacesPerTab() const {
        return spaces_per_tab;
    }

    inline void Indent::setSpacesPerTab(size_t new_spaces_per_tab) {
        spaces_per_tab = new_spaces_per_tab;
    }
} // namespace hi::parser::himl