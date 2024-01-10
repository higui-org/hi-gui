#include "parser/html.h"

namespace hi
{

bool HIML::read(const std::string& filename) noexcept {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Помилка: Не вдалося відкрити файл " << filename << std::endl;
            return false;
        }

        std::string line;
        std::stack<Tag::Pointer> tag_stack;
        int line_number = 0;

        while (std::getline(file, line)) {
            line_number++;


            int indentation;
            // Обробка відступів та вкладеності тегів
            try {
                indentation = CountIndentation(line, line_number);
            } catch (const std::runtime_error& e) {
                std::cerr << filename << ":" << line_number << ": помилка: " << e.what() << "\n";
                std::cerr << line << "\n";
                std::cerr << std::string(indentation, ' ') << "^" << std::endl;
                return false;
            }


            std::regex tag_pattern("^\\s*(\\w+)(?:\\s+!([^\\s]+))?");
            std::regex attr_pattern("^\\s*>\\s*(\\w+)\\s+\"([^\"]+)\"");
            std::smatch match;

            if (std::regex_search(line, match, tag_pattern)) {
                std::string tag_name = match[1];
                auto new_tag = std::make_shared<Tag>(tag_name);

                if (match[2].matched) {
                    new_tag->setId(match[2]);
                }

                if (!tag_stack.empty()) {
                    tag_stack.top()->addChild(new_tag);
                } else {
                    getTagManager().setRoot(new_tag);
                }
                tag_stack.push(new_tag);
            } else if (std::regex_search(line, match, attr_pattern)) {
                if (!tag_stack.empty()) {
                    tag_stack.top()->setAttribute(match[1], match[2]);
                }
            }
        }

        file.close();
        return true;
    }


int HIML::CountIndentation(const std::string& line, int line_number) {
    int count = 0;
    for (int i = 0; i < line.size(); ++i) 
    {
        char ch = line[i];
        
        if (ch == '\t')
        {
            count++;
        } 
        else if (ch == ' ') 
        {
            count++;
            if (count % 2 != 0) {
                std::stringstream ss;
                ss << "Непослідовне використання відступів: очікується табуляція або 2 пробіли на позиції " << (i + 1);
                throw std::runtime_error(ss.str());
            }
        } 
        else { break; }
    }
    return count;
    }


} // namespace 'hi'