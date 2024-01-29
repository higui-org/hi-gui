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
 * @version 24.1
 * @date 1/28/2024
 */

#ifndef HiGUI_PARSER_HIML_HIML_H
#define HiGUI_PARSER_HIML_HIML_H

#include <fstream>
#include <sstream>
#include <optional>
#include <set>
#include <map>

#include "higui/parser/parser.h"
#include "higui/parser/himl/indent.h"

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
        /**
         * @brief Reads and parses a HIML file.
         * @param filename The path of the HIML file to be parsed.
         */
        void read(const std::string& filename) override;

    private:
        Tag::Pointer root; ///< The root tag of the parsed HIML structure.
    };

} // namespace hi::parser::himl

#endif // HiGUI_PARSER_HIML_HIML_H