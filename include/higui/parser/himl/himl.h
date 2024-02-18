/**
 * @file himl.h
 * @brief Header file for the HIML class used in HiGUI parsing module.
 *
 * This file contains the declarations of the HIML class, which is part of the
 * HiGUI project. The HIML class is designed to read and parse .himl files,
 * which are specific to the HI Markup Language (HIML). This class is a
 * specialization of the HIML class, designed specifically to handle the
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
 * @date 12/2/2024
 */

#ifndef HiGUI_PARSER_HIML_HIML_H
#define HiGUI_PARSER_HIML_HIML_H

// std
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <optional>

// higui
#include "higui/parser/base.h"         // parser.h
#include "higui/parser/himl/line.h"    // line.h
#include "higui/parser/himl/section.h" // section.h

namespace hi::parser::himl
{
    /**
     * @class HIML (aka Spaghetti)
     * @brief Reads and parses HI Markup Language (HIML) files.
     *
     * This class is responsible for reading and parsing .himl files.
     * It extends the HIML class and implements its read method to handle HIML files.
     * Use the getTagManager() method to access the parsed structure.
     */
    class HIML : public ParserBase
    {
    public:
        HIML(const std::string& filename)
            : filename(filename), scope()
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
        std::vector<Section> sections; ///< Vector storing the sections of the HIML file.
        std::set<std::string> imported_files; ///< Tracks filenames that have been imported to avoid duplication

        Tag::Pointer root; ///< The root tag of the parsed HIML structure.
        Indent scope; ///< Tracks scope based on indentation levels

        // Processes an import directive
        void ProcessImport(std::ifstream& fstream, const std::string& raw_line, const std::string& filename, int& line_number);

        // Processes a section, reading its contents
        void ProcessSection(std::ifstream& fstream, const std::string& raw_line, const std::string& filename, int& line_number);

        std::optional<std::reference_wrapper<const Section>> findSection(const std::string& path) const;
    };
}

#endif // HiGUI_PARSER_HIML_HIML_H