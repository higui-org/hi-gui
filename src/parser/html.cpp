#include "parser/html.h"

namespace hi
{

bool HIML::read(const std::string& filename) noexcept {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return false;
    }

    std::string line;
    std::stack<std::pair<Tag::Pointer, int>> tag_stack;
    int line_number = 0;

    Tag::Pointer current_tag = nullptr;
    int current_indentation = 0;

    while (std::getline(file, line)) {
        line_number++;

        int indentation = CountIndentation(line);
        if (indentation > current_indentation + 1) {
            ShowError(filename, line_number, indentation, "Incorrect indentation", line);
            return false;
        }

        std::regex tag_pattern("^\\s*(\\w+)(?:\\s*!([^\\s]+))?");
        std::regex attr_pattern("^\\s*>\\s*(\\w+)\\s+\"([^\"]+)\"");
        std::smatch match;

        if (std::regex_search(line, match, tag_pattern)) {
            // Close previous tag if indentation decreases
            while (!tag_stack.empty() && tag_stack.top().second >= indentation) {
                tag_stack.pop();
                if (!tag_stack.empty()) {
                    current_tag = tag_stack.top().first;
                    current_indentation = tag_stack.top().second;
                } else {
                    current_tag = nullptr;
                    current_indentation = 0;
                }
            }

            std::string tag_name = match[1];
            auto new_tag = std::make_shared<Tag>(tag_name);

            if (match[2].matched) {
                new_tag->setId(match[2]);
            }

            if (current_tag) {
                current_tag->addChild(new_tag);
            } else {
                getTagManager().setRoot(new_tag);
            }

            tag_stack.push({new_tag, indentation});
            current_tag = new_tag;
            current_indentation = indentation;
        } else if (std::regex_search(line, match, attr_pattern) && current_tag) {
            current_tag->setAttribute(match[1], match[2]);
        } else if (!line.empty()) {
            ShowError(filename, line_number, indentation, "Unrecognized line format", line);
            return false;
        }
    }

    file.close();
    return true;
}

void HIML::ShowError(
    const std::string& filename, 
    int line_number, 
    int position, 
    const std::string& message, 
    const std::string& line) const 
{
    std::cerr << filename << ":" << line_number << ": error: " << message << "\n";
    std::cerr << line << "\n";
    std::cerr << std::string(position, '~') << "^" << std::endl << std::endl;
}


int HIML::CountIndentation(const std::string& line) 
{
    int count = 0;
    for (char ch : line) {
        if (ch == '\t' || ch == ' ') {
            count++;
            break; // Since only one space or tab is allowed
        } else {
            break;
        }
    }
    return count;
}


} // namespace 'hi'