#include "higui/parser/markup.h"

namespace hi
{
void HIML::read(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open())
        throw ParsingException(filename, 0, "Cannot open file");

    std::string line;
    Tag::Pointer current_section = nullptr;
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue;

        // section recognition
        if (line[0] == '[' && line.back() == ']') 
        {
            std::string section_name = line.substr(1, line.size() - 2);
            current_section = std::make_shared<Tag>(section_name);
            getTagManager().setRoot(current_section);
        }
        else if (current_section) 
        {
            // classes and attrs recognition
            if (line[0] == '\t') {  // attrs
                size_t pos = line.find(' ');
                std::string key = line.substr(1, pos - 1);
                std::string value = line.substr(pos + 1);
                current_section->setAttribute(key, value);
            }
            else if (line[0] == '.') {  // classes
                // ...
            }
        }
    }

    file.close();
}

} // namespace 'hi'