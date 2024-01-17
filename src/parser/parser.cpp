#include "higui/parser/parser.h"

namespace hi
{

std::ostream& operator<<(std::ostream& os, const Tag::Pointer& tag) {
    os << tag->getName();
    if (!tag->getId().empty()) {
        os << ", ID: " << tag->getId();
    }
    if (!tag->getClass().empty()) {
        os << ", Class: " << tag->getClass();
    }

    const auto& attributes = tag->getAttributes();
    if (!attributes.empty()) {
        os << ", Attributes: {";
        for (const auto& [key, value] : attributes) {
            os << " " << key << ": " << value << ";";
        }
        os << " }";
    }

    return os;
}

// TagManager::printTree
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


// Parser methods
//

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

void Parser::findAllRecursive(Tag::Pointer current, const std::string& class_val, std::list<Tag::Pointer>& result) {
    if (!current) return;

    if (current->getClass() == class_val)
        result.push_back(current);

    for (const auto& child : current->getChildren()) 
    {
        findAllRecursive(child, class_val, result);
    }
}
//
// end of 'Parser methods'

// class Tag: setters / getters
//
void Tag::setName(const std::string& name) noexcept { this->name = name; }
void Tag::setId(const std::string& id) noexcept { this->id = id; }
void Tag::setClass(const std::string& class_val) noexcept { this->class_val = class_val; }
void Tag::addChild(Tag::Pointer child) noexcept { children.push_back(child); }
void Tag::setAttribute(const std::string& key, const std::string& value) noexcept { attributes[key] = value; }

std::string Tag::getName() const noexcept { return name; }
std::string Tag::getId() const noexcept { return id; }
std::string Tag::getClass() const noexcept { return class_val; }
std::map<std::string, std::string> Tag::getAttributes() const noexcept { return attributes; }
std::vector<Tag::Pointer> Tag::getChildren() const noexcept { return children; }

//
// end of 'class Tag: setters / getters'

} // namespace 'hi'