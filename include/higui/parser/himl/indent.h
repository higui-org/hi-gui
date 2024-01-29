/**
 * @file Indent.h
 * @brief Header file for the Indent class within the HiGUI HIML parser module.
 *
 * This file contains the declarations of the Indent class, which is a part of the
 * HiGUI project's HIML (HI Markup Language) parsing subsystem. The Indent class is
 * designed to represent and manage the indentation in HIML files. It is capable
 * of handling different types of indentation, such as tabs or spaces, and provides
 * functionality to determine and adjust the indentation level and type.
 *
 * The class encapsulates details such as the type of indentation (tab or spaces),
 * the number of spaces equivalent to a tab, and the count of indentations. This
 * functionality is crucial for parsing HIML files where indentation may signify
 * hierarchical structures.
 *
 * Inclusion of "higui/parser/parser.h" is for accessing the ParsingException class,
 * which may be utilized within the Indent class for error handling.
 *
 * @author setbe
 * @version 24.1
 * @date 1/28/2024
 */

#ifndef HiGUI_PARSER_HIML_INDENT_H
#define HiGUI_PARSER_HIML_INDENT_H

#include <string>

#include "higui/parser/parser.h" // include for ParsingException only.

/*
// Example Usage of Indent Class

#include "higui/parser/himl/himl.h"

#include <iostream>

using namespace hi;
using namespace parser;
using namespace parser::himl;

int main() {
    try {
        // Creating a base Indent object with a specific number of spaces per tab
        Indent base(2); // Assumes 2 spaces equal one tab
        // "base" would have 0 tabs

        // Creating another Indent object using a string, base indent,
        // (filename, and line number provides info for ParsingException, they are not mandatory)
        // This example uses a line with 4 leading spaces
        Indent custom("    Line with leading spaces", base, "example_file.himl", 5);
        // "custom" would have 2 tabs of total, 'cause "base" declares 2 spaces as 1 tab.

        Indent custom_one_tab("    Line with leading spaces", Indent(), "example_file.himl", 5);
        // "custom_one_tab" would have 1 tab of total,
        // 'cause default constructor for param "base_indent" initialized 4 spaces for 1 tab.
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
     * @class Indent
     * @brief A class for internal development within higui.
     *
     * The Indent class represents the type of indentation used (TAB or 1-4 spaces as tab)
     * and the count of indentations. It includes "spaces_per_tab" which indicates the
     * number of spaces equivalent to one tab, assuming Indent::Type is Spaces.
     * By default, 1 TAB is considered equal to 4 spaces.
     */
    class Indent
    {
    public:
        /**
         * @brief Constructor for creating an Indent object.
         * @param spaces_per_tab The number of spaces that represent one complete tab.
         *                       Defaults to 4.
         */
        explicit Indent(size_t spaces_per_tab = 4);
        /**
         * @brief Constructor for creating an Indent object.
         * @param line_with_indents A single line of markup.
         * @param base_indent Contains the number of spaces equivalent to a tab character from the given line
         * @param filename The name of the file containing the line.
         * @param line_num The line number in the file.
         */
        explicit Indent(
            const std::string& line_with_indents, 
            Indent base_indent = Indent(),
            const std::string& filename = "",
            int line_num = -1);

        /**
         * @brief Gets the count of total full tabs.
         * @return The total tabs of indentations.
         */
        size_t getTabs() const;

        /**
         * @brief Gets the number of spaces representing one complete tab.
         * @return The number of spaces per tab.
         */
        size_t getSpacesPerTab() const;

        /**
         * @brief Sets the number of spaces that represent one complete tab.
         * @param newSpacesPerTab The new number of spaces per tab.
         */
        void setSpacesPerTab(size_t newSpacesPerTab);

    private:
        size_t total_tabs; ///< The count of indentations.
        size_t spaces_per_tab; ///< The number of spaces equivalent to one tab.
    };

} // namespace hi::parser::himl
#endif // HiGUI_PARSER_HIML_INDENT_H
