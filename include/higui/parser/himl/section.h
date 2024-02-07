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
 * @version 24.1
 * @date 1/30/2024
 */

#ifndef HiGUI_PARSER_HIML_SECTION_H
#define HiGUI_PARSER_HIML_SECTION_H

#include <string>
#include <vector>
#include <iterator>

#include "higui/parser/parser.h" // include for ParsingException only.
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
         * @brief Constructor for creating a Section object.
         * @param line The line containing the section's name in the format 'section [name]'.
         * @param filename The filename of the HIML document.
         */
        Section(const std::string& name, std::vector<Line> lines = std::vector<Line>{})
            : name(name), lines(lines)
        {}

        /**
         * @brief Retrieves the name of the section.
         * @return The name of the section.
         */
        std::string getName() const;

        void AddLine(const Line& line) { lines.push_back(line); }

        const std::vector<Line>& getLines() const { return lines; }

        /**
         * @brief Iterator for iterating over the indentations.
         * @return Iterator to the beginning of the indentations.
         */
        std::vector<Line>::iterator begin();

        /**
         * @brief Iterator for iterating over the indentations.
         * @return Iterator to the end of the indentations.
         */
        std::vector<Line>::iterator end();

    private:
        std::string name; ///< The name of the section.
        std::vector<Line> lines;
    };

} // namespace hi::parser::himl
#endif // HiGUI_PARSER_HIML_SECTION_H