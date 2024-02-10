#include "higui/parser/himl/himl.h"

namespace hi::parser::himl
{   
// Reads and parses the Parser file
void Parser::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw ParsingException("Couldn't open HIML file.", filename);

    std::string raw_line;

    while (getline(file, raw_line)) 
    {
        this->line_number++;
        Line line(raw_line, this->scope);
        if (line.StartsWith("["))
        {
            ProcessSection(file, raw_line);
        }
    }
}

// Processes an import directive
void Parser::ProcessImport(std::ifstream& fstream, const std::string& raw_line)
{
    // Implementation to handle import, including reading the specified file and section
}

// Processes a section, reading its contents into the sections map
void Parser::ProcessSection(std::ifstream& fstream, const std::string& section_start_line)
{
    sections.push_back(Section(section_start_line, filename, line_number));

    // The current indentation level for the section is determined by the first line of the section
    bool is_first_line = true;
    std::string raw_line;

    while (std::getline(fstream, raw_line))
    {
        this->line_number++;        // increment line number
        Line line(raw_line, sections.back().getIndent());   // create new Line object within section indent

        // Skip empty lines and detect the first non-empty line to determine indentation            
        if (line.empty()) continue;

        if (is_first_line)
        {
            // Adjust the spaces_per_tab based on the first non-empty line after the section name
            line.indent.setSpacesPerTab(Line::CountLeadingSpaces(raw_line));    // set line indent
            sections.back().setIndent(line.indent); // set section's 'spaces per tab'

            line = Line(raw_line, line.indent);     // determine tabs by creating new object
            is_first_line = false;
        }
        else
        {
            // Check if the line has less indentation, signaling the end of the section
            if (line.indent.getTabs() <= this->scope.getTabs())
            {
                // Move the file cursor back to allow the next method to read this line
                fstream.seekg(-static_cast<int>(line.length()) - 2, std::ios_base::cur);
                this->line_number--;
                break;
            }

            // Adjust the line to the current indentation level in the main document
            line.indent.setTabs(line.indent.getTabs() + this->scope.getTabs());
        }

        // Adding the line to the sections map
        sections.back().AddLine(line);
    }
}
} // namespace hi::parser::himl