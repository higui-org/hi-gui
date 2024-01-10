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

    virtual bool read(const std::string& filename) noexcept = 0;
    bool write(const Tag::Pointer& where, const Tag& tag);
    Tag::Pointer find(const std::string& id) const;
    std::list<Tag::Pointer> findAll(const std::string& class_val) const;

    inline TagManager& getTagManager() const noexcept { return tagManager; }

private:
    mutable TagManager tagManager;  // mutable for const method modifications

    static Tag::Pointer findRecursive(Tag::Pointer current, const std::string& id);

    static void findAllRecursive(
        Tag::Pointer             current, 
        const std::string&       class_val,
        std::list<Tag::Pointer>& result
    );
};

} // namespace hi
#endif // HiGUI_PARSER_H