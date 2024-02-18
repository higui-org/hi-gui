#include "higui/parser/base.h"

namespace hi::parser
{


/*
        HIML methods
    (no getters/setters)
            |
            |
           \ /
*/
bool ParserBase::write(const Tag::Pointer& where, const Tag& tag) {
    if (!where) return false;

    Tag::Pointer newTag = std::make_shared<Tag>(tag);
    where->addChild(newTag);

    return true;
}

Tag::Pointer ParserBase::find(const std::string& id) const
{
    return findRecursive(dom.getRoot(), id);
}

std::list<Tag::Pointer> ParserBase::findAll(const std::string& class_val) const
{
    std::list<Tag::Pointer> result;
    findAllRecursive(dom.getRoot(), class_val, result);
    return result;
}

Tag::Pointer ParserBase::findRecursive(Tag::Pointer current, const std::string& id) {
    if (!current) return nullptr;

    if (current->getId() == id) return current;

    for (const auto& child : current->getChildren())
    {
        Tag::Pointer result = findRecursive(child, id);
        if (result) return result;
    }
    return nullptr;
}

void ParserBase::findAllRecursive(
    Tag::Pointer current,
    const std::string& class_val,
    std::list<Tag::Pointer>& result) {
    if (!current) return;

    for (std::string& class_ : current->getClasses())
    {
        if (class_val == class_)
            result.push_back(current);
    }

    for (const auto& child : current->getChildren())
    {
        findAllRecursive(child, class_val, result);
    }
}


/*
        class Tag
    setters   getters
            |
            |
           \ /

*/

// setters
void Tag::setName(const std::string& name) noexcept
{
    this->name = name;
}

void Tag::setId(const std::string& id) noexcept
{
    this->id = id;
}

void Tag::setClass(const std::string& class_val) noexcept
{
    classes.push_back(class_val);
}

void Tag::addChild(Tag::Pointer child) noexcept
{
    children.push_back(child);
}

void Tag::setAttribute(const std::string& key, const std::string& value) noexcept
{
    attributes[key] = value;
}

void Tag::setText(const std::string& text) noexcept
{
    this->text = text;
}


// getters
std::string Tag::getName() const noexcept
{
    return name;
}

std::string Tag::getId() const noexcept
{
    return id;
}

std::vector<std::string> Tag::getClasses() const noexcept
{
    return classes;
}

std::map<std::string, std::string> Tag::getAttributes() const noexcept
{
    return attributes;
}

std::vector<Tag::Pointer> Tag::getChildren() const noexcept
{
    return children;
}

std::string Tag::getText() const noexcept
{
    return text;
}
/*
               / \
                |
                |
        setters   getters
            class Tag
*/



/*

    class ParsingException
*/
ParsingException::ParsingException(
    const std::string& message,
    const std::string& line,
    const std::string& filename,
    int line_number
)
    : filename(filename),
    line_number(line_number)
{
    full_message = "ParsingException was thrown";                   // "ParsingException was thrown"

    if (!filename.empty())
        full_message += " in file \"" + filename + "\"";            // " in file "<filename>""
    if (line_number >= 0)
        full_message += " on line " + std::to_string(line_number);  // " on line <line_number>"

    full_message += ":\n\t" + message;      // ":<new line><tab><message>"

    if (!line.empty())
        full_message += "\n\t\"" + line + "\"";                     // "<new line><tab>"line""
}

const char* ParsingException::what() const noexcept
{
    return full_message.c_str();
}

const std::string& ParsingException::getFilename() const noexcept
{
    return filename;
}

int ParsingException::getLineNumber() const noexcept
{
    return line_number;
}


} // namespace 'hi::parser'