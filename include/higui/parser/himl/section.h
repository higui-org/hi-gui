/**
 * @file section.h
 * @brief Header file for the Section class within the HiGUI HIML parser module.
 *
 * This file contains the declarations of the Section class, which is part of the
 * HiGUI project's HIML (HI Markup Language) parsing subsystem. The Section class
 * is designed to represent a distinct section within a HIML document. It handles
 * the parsing and storage of the section's content, managing the section's name,
 * its associated filename, and the indentation levels of its content.
 *
 * The Section class plays a crucial role in interpreting the structure of HIML
 * documents, allowing for a clear representation and manipulation of hierarchical
 * data within these documents. It utilizes the Indent class for managing line
 * indentations and may throw ParsingException in cases of syntax errors or
 * when section names are improperly defined.
 *
 * @author setbe
 * @version 24.2
 * @date 10/2/2024
 */

#ifndef HiGUI_PARSER_HIML_SECTION_H
#define HiGUI_PARSER_HIML_SECTION_H

#include <string>
#include <vector>
#include <iterator>

#include "higui/parser/base.h" // include for ParsingException only.
#include "higui/parser/himl/line.h"

namespace hi::parser::himl
{
/**
    * @class Section
    * @brief Represents a section within a HIML document.
    *
    * This class encapsulates a section within a HIML (HI Markup Language) file.
    * It processes a line containing the section's name in the format 'section [name]'
    * and provides functionality to access the section's name, iterate over its lines,
    * and retrieve the associated filename.
    */
class Section
{
public:
    /**
    * @brief A class for internal development within higui. Constructs a Section object by parsing a line for the section name.
    *
    * This constructor takes a line expected to contain the section name enclosed in square brackets
    * (e.g., "[name]") and initializes the Section object with this name. If the line does not conform
    * to the expected format, a ParsingException is thrown. The filename and line number are used in
    * the exception message to help identify the location of the error in the HIML document.
    *
    * @param line A string representing the line from which to extract the section name. The section
    * name within the line must be enclosed in square brackets.
    * @param filename (Optional) The name of the HIML document from which the line is extracted. This
    * is used for error reporting to provide context about where the parsing error occurred. If not
    * provided, a default empty string is used.
    * @param line_num (Optional) The line number in the HIML document where the line is located. This
    * is used for error reporting to pinpoint the exact location of the parsing error. If not provided,
    * a default value of -1 is used.
    *
    * @throws ParsingException If the line does not contain a properly formatted section name. The
    * exception message includes the filename and line number (if provided) to aid in diagnosing the
    * parsing error.
    */
    Section(const std::string& raw_line, const std::string& filename = "", int line_num = -1)
        : lines{}, indent()
    {
        this->name = Section::ExtractName(raw_line, filename, line_num);
    }

    /**
    * @brief Adds a new line to the section.
    *
    * This method takes a Line object as input and appends it to the end of the
    * 'lines' container. It is used to populate a Section with content line by line.
    *
    * @param line The Line object to be added to the section. It represents a single
    * line of text or information that belongs within the section.
    */
    void AddLine(const Line& line) { lines.push_back(line); }

    /**
    * @brief Extracts the section name from a given line.
    *
    * This static method parses a line to extract the section name, which is expected
    * to be enclosed in square brackets ('[' and ']'). The method trims whitespace
    * from the extracted name and validates that the name is not empty.
    *
    * @param line The line from which to extract the section name. This line should
    * follow the format '[section_name]'.
    *
    * @param filename The name of the file from which the line is taken. This is used
    * for error reporting purposes. Default value is an empty string, indicating that
    * the file name is not provided or not relevant.
    *
    * @param line_num The line number in the file. This is used for error reporting
    * purposes. Default value is -1, indicating that the line number is not provided
    * or not relevant.
    *
    * @return std::string The extracted section name, trimmed of any leading or trailing
    * whitespace.
    *
    * @throws ParsingException If the line does not contain a properly formatted
    * section name, a ParsingException is thrown. The exception includes the message
    * about the error, the file name, and the line number (if provided).
    */
    static std::string ExtractName(const std::string& raw_line, const std::string& filename = "", int line_num = -1);


    /*
        iterators
    setters   getters
            |
            |
            \ /
    */
    /**
    * @brief Retrieves the name of the section.
    * @return The name of the section.
    */
    std::string getName() const noexcept;

    /**
    * @brief Sets the indentation for the section.
    *
    * This method assigns a specified Indent object to the section, which determines
    * the indentation level for the lines contained within the section.
    *
    * @param indent The Indent object specifying the indentation settings to be applied
    * to the section.
    */
    void setIndent(Indent indent) noexcept;

    /**
    * @brief Retrieves the current indentation settings of the section.
    *
    * This method returns the Indent object associated with the section, representing
    * the current indentation settings.
    *
    * @return Indent The current indentation settings of the section.
    */
    Indent getIndent() const noexcept { return indent; }

    /**
    * @brief Provides read-only access to the lines contained within the section.
    *
    * This method returns a constant reference to the internal vector of Line objects
    * that make up the content of the section, allowing read-only access.
    *
    * @return const std::vector<Line>& A constant reference to the vector of Line
    * objects contained within the section.
    */
    const std::vector<Line>& getLines() const noexcept { return lines; }

    /**
    * @brief Iterator for iterating over the indentations.
    * @return Iterator to the beginning of the indentations.
    */
    std::vector<Line>::iterator begin() noexcept;

    /**
    * @brief Iterator for iterating over the indentations.
    * @return Iterator to the end of the indentations.
    */
    std::vector<Line>::iterator end() noexcept;

    /**
    * @brief Const iterator for iterating over the indentations.
    * @return Const iterator to the beginning of the indentations.
    */
    std::vector<Line>::const_iterator begin() const noexcept;

    /**
    * @brief Const iterator for iterating over the indentations.
    * @return Const iterator to the end of the indentations.
    */
    std::vector<Line>::const_iterator end() const noexcept;

    /*
            / \
            |
            |
    setter     getters
        iterators
    */


private:
    Indent indent; ///< Object for managing indentation for all lines.
    std::string name; ///< The name of the section.
    std::vector<Line> lines; ///< Container holding all lines within the section.
};

} // namespace hi::parser::himl
#endif // HiGUI_PARSER_HIML_SECTION_H