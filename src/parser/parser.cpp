#include "higui/parser/parser.h"

namespace hi::parser
{
/*
         Tag Manager    :print methods

              |
              |
             \ /
*/
void TagManager::printTree(int depth) const noexcept
{
    printTree(root, depth);
}

void TagManager::printTree(const Tag::Pointer& tag, int depth) const noexcept
{
    if (!tag) return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << tag << std::endl;

    for (const auto& child : tag->getChildren()) {
        printTree(child, depth + 1);
    }
    std::cout << indent << "/" << tag->getName() << std::endl;
}


/*
            Parser     :various methods (no getters/setters)

              |
              |
             \ /
*/
bool Parser::write(const Tag::Pointer& where, const Tag& tag) {
    if (!where) return false;

    Tag::Pointer newTag = std::make_shared<Tag>(tag);
    where->addChild(newTag);

    return true;
}

Tag::Pointer Parser::find(const std::string& id) const
{
    return findRecursive(getTagManager().getRoot(), id);
}

std::list<Tag::Pointer> Parser::findAll(const std::string& class_val) const
{
    std::list<Tag::Pointer> result;
    findAllRecursive(getTagManager().getRoot(), class_val, result);
    return result;
}

Tag::Pointer Parser::findRecursive(Tag::Pointer current, const std::string& id) {
    if (!current) return nullptr;

    if (current->getId() == id) return current;

    for (const auto& child : current->getChildren())
    {
        Tag::Pointer result = findRecursive(child, id);
        if (result) return result;
    }
    return nullptr;
}

void Parser::findAllRecursive(
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

      SETTERS   GETTERS   ( various classes )
              |
              |
             \ /


*//*

 class: Tag
*/
void Tag::setName(const std::string& name) noexcept
{ this->name = name; }

void Tag::setId(const std::string& id) noexcept 
{ this->id = id; }

void Tag::setClass(const std::string& class_val) noexcept 
{ classes.push_back(class_val); }

void Tag::addChild(Tag::Pointer child) noexcept 
{ children.push_back(child); }

void Tag::setAttribute(const std::string& key, const std::string& value) noexcept 
{ attributes[key] = value; }

void Tag::setText(const std::string& text) noexcept 
{ this->text = text;  }

std::string Tag::getName() const noexcept 
{ return name; }

std::string Tag::getId() const noexcept 
{ return id; }

std::vector<std::string> Tag::getClasses() const noexcept 
{ return classes; }

std::map<std::string, std::string> Tag::getAttributes() const noexcept 
{ return attributes; }

std::vector<Tag::Pointer> Tag::getChildren() const noexcept 
{ return children; }

std::string Tag::getText() const noexcept 
{ return text; }
/*
 
 TagManager  
*/
TagManager& Parser::getTagManager() const noexcept 
{ return tag_manager; }

void TagManager::setRoot(Tag::Pointer root) noexcept
{
    this->root = root;
}

Tag::Pointer TagManager::getRoot() const noexcept
{
    return this->root;
}

/*
             / \
              |
              |
      SETTERS   GETTERS
*/


/*

 class: ParsingException
*/
ParsingException::ParsingException(
    const std::string& message,
    const std::string& filename,
    int line_number
    )
    : filename(filename),
    line_number(line_number)
{
    full_message = "ParsingException was thrown ";
    if (!filename.empty()) full_message += "in file \"" + filename + "\" ";
    if (line_number >= 0) full_message += "on line " + std::to_string(line_number);
    full_message += "\nWith message: " + message;
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




/*
  
 reloaded '<<' for Tag output
*/
std::ostream& operator<<(std::ostream& os, const Tag::Pointer& tag) 
{
    // Check if the tag object exists. If it doesn't, output "Null Tag" and a new line.
    if (!tag) {
        os << "Null Tag\n";
        return os;
    }

    // Output the name of the tag.
    os << "Tag: " << tag->getName() << "\n";

    // Check if the ID of the tag is not empty, and output it.
    if (!tag->getId().empty()) {
        os << "ID: " << tag->getId() << "\n";
    }

    // Check if the text of the tag is not empty, and output it.
    if (!tag->getText().empty()) {
        os << "Text: " << tag->getText() << "\n";
    }
                    
    // Check if the classes of the tag are not empty, and output them.
    // Loop through each class name and output it followed by a space.
    if (!tag->getClasses().empty()) {
        os << "Classes: ";
        for (const auto& class_name : tag->getClasses()) {
            os << class_name << " ";
        }
        os << "\n"; // Add a new line at the end
    }

    return os;
}

} // namespace 'hi::parse'