/**
 * @file HIML.h
 * @brief Header file for the HIML class used in HiGUI parsing module.
 *
 * This file contains the declarations of the HIML class, which is part of the
 * HiGUI project. The HIML class is designed to read and parse .himl files,
 * which are specific to the HI Markup Language (HIML). This class is a
 * specialization of the Parser class, designed specifically to handle the
 * nuances of HIML syntax and structure.
 *
 * The HIML parser facilitates the interpretation of HIML files, converting
 * them into a structured format that can be utilized by the HiGUI framework.
 * This file includes necessary standard libraries and dependencies, defines
 * the hi::parser::himl namespace, and declares the HIML class with its
 * associated methods and members.
 *
 * @author setbe
 * @version 24.2
 * @date 2/2/2024
 */

#ifndef HiGUI_PARSER_HIML_HIML_H
#define HiGUI_PARSER_HIML_HIML_H

// std
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>

// higuid
#include "higui/parser/parser.h"        // parser.h
#include "higui/parser/himl/line.h"     // line.h
#include "higui/parser/himl/section.h"  // section.h

namespace hi::parser::himl
{
    /**
     * @class HIML
     * @brief Reads and parses HI Markup Language (HIML) files.
     *
     * This class is responsible for reading and parsing .himl files.
     * It extends the Parser class and implements its read method to handle HIML files.
     * Use the getTagManager() method to access the parsed structure.
     */
    class HIML : public Parser
    {
    public:
        HIML(const std::string& filename)
            : filename(filename), scope(0)
        {
            read(filename);
        }
        /**
         * @brief Reads and parses a HIML file.
         * @param filename The path of the HIML file to be parsed.
         */
        void read(const std::string& filename) override;

    private:
        std::string filename; ///< The filename of the HIML document.
        std::vector<Section> sections; ///< Map storing the sections of the HIML file.
        std::set<std::string> imported_files; ///< Tracks filenames that have been imported to avoid duplication

        Tag::Pointer root; ///< The root tag of the parsed HIML structure.
        Line scope; ///< Tracks scope based on indentation levels

        // Extracts a section or tag name from a line
        std::string ExtractName(const Line& line, int line_num = -1);

        // Processes an import directive
        void ProcessImport(std::ifstream& fstream, const std::string& section_name, int line_num = -1);

        // Processes a section, reading its contents
        void ProcessSection(std::ifstream& fstream, const std::string& section_name, int line_num = -1);
    };
}

#endif // HiGUI_PARSER_HIML_HIML_H