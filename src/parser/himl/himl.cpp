#include "higui/parser/himl/himl.h"

namespace hi::parser::himl
{   
    // Reads and parses the HIML file
    void HIML::read(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw ParsingException("Couldn't open HIML file.", filename);
        }

        Line line;
        while (getline(file, line)) {
            if (line.StartsWith("["))
            {
                std::string sec_name = ExtractName(line);
                ProcessSection(file, sec_name);
            }
        }
    }

    // Processes an import directive
    void HIML::ProcessImport(std::ifstream& fstream, const std::string& section_name, int line_num) {
        // Implementation to handle import, including reading the specified file and section
    }

    // Processes a section, reading its contents into the sections map
    void HIML::ProcessSection(std::ifstream& fstream, const std::string& section_name, int line_num) {

        Line line;
        std::vector<Line> section_lines;

        sections.push_back(Section(section_name));

        // The current indentation level for the section is determined by the first line of the section
        size_t section_indentation_tabs = 0;
        bool is_first_line = true;

        while (std::getline(fstream, line)) {
            // Skip empty lines and detect the first non-empty line to determine indentation            
            Line processed_line(line);

            if (processed_line.empty()) continue;

            if (is_first_line) {
                // Adjust the spaces_per_tab based on the first non-empty line after the section name
                size_t spaces_n = line.CountLeadingSpaces();
                
                is_first_line = false;
            }
            else {
                // Check if the line has less indentation, signaling the end of the section
                if (processed_line.getTabs() <= section_indentation_tabs) {
                    // Move the file cursor back to allow the next method to read this line
                    fstream.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
                    break;
                }

                // Adjust the line to the current indentation level in the main document
                processed_line.setTabs(processed_line.getTabs() + scope.getTabs() - section_indentation_tabs);
            }

            // Adding the line to the sections map
            sections.back().AddLine(processed_line);
        }
    }

    // Private method for extracting the section name from the line
    std::string HIML::ExtractName(const Line& line, int line_num)
    {
        // Assuming the format is '[name]', extract the name part
        size_t start_bracket = line.find('[');
        size_t end_bracket = line.find(']', start_bracket);

        if (start_bracket == std::string::npos || end_bracket == std::string::npos) {
            throw ParsingException("Syntax for section name is incorrect.", filename);
        }

        std::string name = line.substr(start_bracket + 1, end_bracket - start_bracket - 1);

        Line temp(name);
        temp.TrimSpaces();
        name = temp;

        if (name.empty()) {
            throw ParsingException("Section name not found.", filename, line_num);
        }

        return name;
    }
}