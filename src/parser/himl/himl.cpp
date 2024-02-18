#include "higui/parser/himl/himl.h"

namespace hi::parser::himl
{   
// Reads and parses the HIML file
void HIML::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw ParsingException("Couldn't open HIML file.", "", filename);

    std::string raw_line;
    int line_number = 0;

    getline(file, raw_line);
    if (raw_line.size() >= 3 &&
        (unsigned char)raw_line[0] == 0xEF &&
        (unsigned char)raw_line[1] == 0xBB &&
        (unsigned char)raw_line[2] == 0xBF) {
        // remove BOM
        raw_line = raw_line.substr(3);
    }

    do {
        line_number++;
        Line line(raw_line, this->scope);

        if (line.StartsWith("import"))
        {
            ProcessImport(file, raw_line, filename, line_number);
        }
        else if (line.StartsWith("["))
        {
            ProcessSection(file, raw_line, filename, line_number);
        }
        else if (line.StartsWith("def"))
        {
            ProcessSection(file, raw_line, filename, line_number);
        }
    } while (getline(file, raw_line));

    file.close();
}

std::optional<std::reference_wrapper<const Section>> HIML::findSection(const std::string& path) const {
    std::istringstream path_stream(path);
    std::string segment;
    std::getline(path_stream, segment, '.');

    const Section* current_section = nullptr;
    for (const auto& section : sections) {
        if (section.getName() == segment) {
            current_section = &section;
            break;
        }
    }

    while (std::getline(path_stream, segment, '.')) {
        if (current_section == nullptr) {
            return std::nullopt; // Path segment not found
        }
        const auto& inline_sections = current_section->getInlineSections();
        const Section* nextSection = nullptr;
        for (const auto& s : inline_sections) {
            if (s.getName() == segment) {
                nextSection = &s;
                break;
            }
        }
        current_section = nextSection;
    }

    if (current_section != nullptr) {
        return *current_section;
    }
    return std::nullopt;
}

// Processes an import directive
void HIML::ProcessImport(std::ifstream& fstream, const std::string& raw_line, const std::string& filename, int& line_number) {
    size_t start_pos = raw_line.find("import") + 6; // "import" has 6 letters
    start_pos = raw_line.find_first_not_of(" ", start_pos);

    if (start_pos == std::string::npos) 
    {
        throw ParsingException("Invalid import syntax", raw_line, filename, line_number);
    }

    std::string import_path = raw_line.substr(start_pos);
    std::replace(import_path.begin(), import_path.end(), '.', '/'); // replace with slashes
    import_path += ".himl"; // add file format

    std::string directory = this->filename.substr(0, this->filename.find_last_of("/\\") + 1);
    std::string full_import_path = directory + import_path;

    // Check imported
    if (imported_files.find(full_import_path) != imported_files.end()) {
        // ignore imported
        return;
    }

    // add imported
    imported_files.insert(full_import_path);

    // recursive read file
    read(full_import_path);
}

// Processes a section, reading its contents into the sections map
void HIML::ProcessSection(std::ifstream& fstream, const std::string& section_start_line, const std::string& filename, int& line_number)
{
    sections.push_back(Section(section_start_line, filename, line_number));

    // The current indentation level for the section is determined by the first line of the section
    bool is_first_line = true;
    std::string raw_line;

    while (std::getline(fstream, raw_line))
    {
        line_number++;        // increment line number
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
                line_number--;
                break;
            }

            // Adjust the line to the current indentation level in the main document
            line.indent.setTabs(line.indent.getTabs() + this->scope.getTabs());
        }

        // Adding the line to the sections map
        sections.back().AddLine(line);
    }
    printf("%s\n", sections.back().getName().c_str());
    for (auto& l : sections.back())
    {
        printf("%s %s\n", std::string(l.indent.getTabs(), '\t').c_str(), l.c_str());
    }
}
} // namespace hi::parser::himl