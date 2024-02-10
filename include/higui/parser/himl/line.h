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
 * @version 24.2
 * @date 10/2/2024
 */

#ifndef HiGUI_PARSER_HIML_LINE_H
#define HiGUI_PARSER_HIML_LINE_H

#include <string>
#include <algorithm> // for std::count

#include "higui/parser/base.h" // include for ParsingException only.


namespace hi::parser::himl
{

/**
* @class Indent
* @brief A class for internal development within higui. Manages text indentation.
*
* The Indent class represents the type of indentation (TAB or 1-4 spaces in place of a tab)
* and the number of indentations. It includes `spaces_per_tab`, which indicates
* the number of spaces equivalent to one tab.
*/
class Indent 
{
public:
/**
* @brief Constructor for creating an Indent object.
* @param spaces_per_tab Number of spaces representing one complete tab.
*                       Defaults to 4.
*/
explicit Indent(size_t spaces_per_tab = 4)
    : tabs(0), spaces_per_tab(spaces_per_tab)
{}

/**
* @brief Counts the number of tabs based on the given string.
* @param str String to analyze.
* @return The number of full tabs in the string.
*/
void DetermineTabsFrom(const std::string& str) noexcept;

/**
* @brief Sets the number of spaces representing one complete tab.
* @param spaces_per_tab The new number of spaces per tab.
*/
void setSpacesPerTab(size_t spaces_per_tab) noexcept { this->spaces_per_tab = spaces_per_tab; }

/**
* @brief Gets the number of spaces representing one complete tab.
* @return The number of spaces per tab.
*/
size_t getSpacesPerTab() const noexcept { return spaces_per_tab; }

void setTabs(size_t tabs_num) noexcept { tabs = tabs_num; }

size_t getTabs() const noexcept { return tabs; }

private:
size_t spaces_per_tab; ///< Number of spaces equivalent to one tab.
size_t tabs;
};

/**
* @class Line
* @brief A class for internal development within higui.
*
* The Line class represents the type of indentation used (TAB or 1-4 spaces as a tab)
* and the count of indentations. It includes `spaces_per_tab` to indicate
* the number of spaces equivalent to one tab, assuming Indent::Type is Spaces.
* By default, 1 TAB is considered equal to 4 spaces.
*/
class Line : public std::string {
public:
Indent indent; ///< Object for managing indentation.

explicit Line() = delete;

/**
* @brief Constructor for creating a Line object.
* @param line A single line of markup.
* @param spaces_per_tab Indicates the number of spaces equivalent to a tab character from the given line.
*/
Line(const std::string& line, Indent indent);

bool StartsWith(const std::string& str) const noexcept;
        
static std::string Trim(const std::string& str) noexcept;
static size_t CountLeadingSpaces(const std::string& str) noexcept;
};

} // namespace hi::parser::himl
#endif // HiGUI_PARSER_HIML_LINE_H
