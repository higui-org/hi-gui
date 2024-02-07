/**
 * @file line.h
 * @brief Header file for the Line class within the HiGUI HIML parser module.
 *
 * This file contains the declarations of the Line class, which is a part of the
 * HiGUI project's HIML (HI Markup Language) parsing subsystem. The Line class is
 * designed to represent the line of text and manage the indentation in HIML files. 
 * It is capable of handling different types of indentation, such as tabs or spaces, 
 * and provides functionality to determine and adjust the indentation level and type.
 *
 * The class encapsulates details such as the type of indentation (tab or spaces),
 * the number of spaces equivalent to a tab, text line, and the count of indentations.
 * This functionality is crucial for parsing HIML files where indentation may signify
 * hierarchical structures.
 *
 * Inclusion of "higui/parser/parser.h" is for accessing the ParsingException class,
 * which may be utilized within the Indent class for error handling.
 *
 * @author setbe
 * @version 24.1
 * @date 1/30/2024
 */

#ifndef HiGUI_PARSER_HIML_LINE_H
#define HiGUI_PARSER_HIML_LINE_H

#include <string>

#include "higui/parser/parser.h" // include for ParsingException only.

/*
// Example Usage of Line Class

#include "higui/parser/himl/line.h"

#include <iostream>

using namespace hi;
using namespace parser;

int main() {
    std::string filename = "example_file.himl";
    size_t line_number = 5;

    try {
        // Counting the number of leading spaces in the line, which is assumed to be equivalent to one tab.
        // In this case, we are assuming 4 spaces equal one tab.
        size_t tab_line_size = himl::Line::CountLeadingSpaces("    line with one tab", filename, line_number);

        // Creating an Indent object using a string and the previously calculated tab line size.
        // This example uses a line with 4 leading spaces, which, according to our previous assumption, is considered one tab.
        std::string custom_line = "    Line with leading spaces";

        Line custom(custom_line, tab_line_size, filename, line_number);
        // The "custom" object will have 1 tab in total, as tab_line_size declares 4 spaces as 1 tab.
    }
    catch (const ParsingException& e) {
        std::cerr << "Parsing exception: " << e.what() << std::endl;
    }

    return 0;
}
*/

namespace hi::parser::himl
{
    /**
     * @class Line
     * @brief A class for internal development within higui.
     *
     * The Line class represents the type of indentation used (TAB or 1-4 spaces as tab)
     * and the count of indentations. It includes "spaces_per_tab" which indicates the
     * number of spaces equivalent to one tab, assuming Indent::Type is Spaces.
     * By default, 1 TAB is considered equal to 4 spaces.
     */

    class Line : public std::string
    {
    public:
        /**
         * @brief Constructor for creating an Line object.
         * @param spaces_per_tab The number of spaces that represent one complete tab.
         *                       Defaults to 4.
         */
        explicit Line(size_t spaces_per_tab = 4);


        /**
         * @brief Constructor for creating an Line object.
         * @param line A single line of markup.
         * @param spaces_per_tab Indicates the number of spaces equivalent to a tab character from the given line.
         * @param filename The name of the file containing the line.
         * @param line_num The line number in the file.
         */
        explicit Line(
            const std::string& line,
            size_t spaces_per_tab = 4,
            const std::string& filename = "",
            int line_num = -1
        );



        /**
         * @brief CountLeadingSpaces counts leading spaces.
         * @return The number of leading spaces in the line.
         */
        size_t CountLeadingSpaces() const noexcept;

        static size_t CountLeadingSpaces(const std::string& str) noexcept;

        /**
        * @brief Trims leading and trailing spaces in line.
        */
        std::string TrimSpaces() noexcept;

        static std::string TrimSpaces(const std::string& str) noexcept;



        bool StartsWith(const std::string& str) const noexcept;


        /**
         * @brief getTabs gets the count of total full tabs.
         * @return The total tabs of indentations.
         */
        size_t getTabs() const noexcept;

        /**
         * @brief setTabs sets the number of total full tabs.
         * @param tabs_n The new number of spaces per tab.
         */
        void setTabs(size_t tabs) noexcept;

        /**
         * @brief getSpacesPerTab gets the number of spaces representing one complete tab.
         * @return The number of spaces per tab.
         */
        size_t getSpacesPerTab() const noexcept;

        /**
         * @brief getLine gets the line without tabs.
         * @return The line without tabs.
         */
        std::string getLine() const noexcept;

        /**
         * @brief setSpacesPerTab sets the number of spaces that represent one complete tab.
         * @param spaces_per_tab The new number of spaces per tab.
         */
        void setSpacesPerTab(size_t spaces_per_tab) noexcept;

    private:
        size_t total_tabs;     ///< The count of indentations.
        size_t spaces_per_tab; ///< The number of spaces equivalent to one tab.
    };

} // namespace hi::parser::himl
#endif // HiGUI_PARSER_HIML_LINE_H
