#ifndef HiGUI_PARSER_H
#define HiGUI_PARSER_H

#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>

namespace hi 
{

class Tag {
public:
    using Pointer = std::shared_ptr<Tag>;
    using WeakPointer = std::weak_ptr<Tag>;

    explicit Tag(const std::string& name) : name(name) {}

    // setters
    void setName(const std::string& name) noexcept;
    void setId(const std::string& id) noexcept;
    void setClass(const std::string& class_val) noexcept;
    void addChild(Pointer child) noexcept;
    void setAttribute(const std::string& key, const std::string& value) noexcept;
    // -------

    // getters
    std::string getName() const noexcept;
    std::string getId() const noexcept;
    std::string getClass() const noexcept;
    std::map<std::string, std::string> getAttributes() const noexcept;
    std::vector<Pointer> getChildren() const noexcept;
    // -------

private:
    std::string name;
    std::string id;
    std::string class_val;
    std::map<std::string, std::string> attributes;
    std::vector<Pointer> children;
    WeakPointer parent;
};

std::ostream& operator<<(std::ostream& os, const Tag::Pointer& tag);

class TagManager {
public:
    inline void setRoot(Tag::Pointer root) noexcept { this->root = root; }
    inline Tag::Pointer getRoot() const noexcept { return root; }

    void printTree(const Tag::Pointer& tag, int depth = 0) const noexcept;
    void printTree(int depth = 0) const noexcept;

private:
    mutable Tag::Pointer root;  // mutable for const method modifications
};

class Parser {
public:
    Parser() noexcept = default;
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    virtual void read(const std::string& filename) = 0;
    bool write(const Tag::Pointer& where, const Tag& tag);
    Tag::Pointer find(const std::string& id) const;
    std::list<Tag::Pointer> findAll(const std::string& class_val) const;

    inline TagManager& getTagManager() const noexcept { return tag_manager; }

private:
    mutable TagManager tag_manager;  // mutable for const method modifications

    static Tag::Pointer findRecursive(Tag::Pointer current, const std::string& id);

    static void findAllRecursive(
        Tag::Pointer             current, 
        const std::string&       class_val,
        std::list<Tag::Pointer>& result
    );
};

class ParsingException : public std::exception
{
public:
    ParsingException
    (
        const std::string& filename,
        int line_number,
        const std::string& message
    ) : filename(filename), line_number(line_number), message(message) {}

    const char* what() const noexcept override
    {
        what_str = (filename + ": line" + std::to_string(line_number) + ": " + message).c_str();
        return what_str;
    }

    const std::string& getFilename() const noexcept { return filename; }

    int getLineNumber() const noexcept { return line_number; }

private:
    std::string filename;
    std::string message;
    int line_number;
    mutable const char* what_str;
};

} // namespace hi
#endif // HiGUI_PARSER_H