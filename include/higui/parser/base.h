/**
 * @file Parser.h
 * @brief Header file defining the base classes for the HiGUI parser module.
 *
 * This file contains the class definitions for the HiGUI project's parser module.
 * It includes the Tag, TagManager, Parser, and ParsingException classes,
 * which are integral to the HiGUI parsing system. These classes provide a
 * framework for parsing, managing, and handling exceptions for HTML-like structured data.
 *
 * @author setbe
 * @version 24.2
 * @date 10/2/2024
 */

#ifndef HiGUI_PARSER_BASE_H
#define HiGUI_PARSER_BASE_H

#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <functional>

namespace hi::parser
{
    /**
     * @class Tag
     * @brief Represents a tag in the parsed structure.
     *
     * This class encapsulates the properties of a tag, including its name,
     * id, class, children, and attributes. It provides methods to manipulate
     * these properties and manage the hierarchy of tags.
     */
    class Tag
    {
    public:
        using Pointer = std::shared_ptr<Tag>;
        using WeakPointer = std::weak_ptr<Tag>;

        /**
         * @brief Construct a new Tag object.
         * @param name The name of the tag.
         */
        explicit Tag(const std::string& name) : name(name) {}

        // setters
        void setName(const std::string& name) noexcept;
        void setId(const std::string& id) noexcept;
        void setClass(const std::string& class_val) noexcept;
        void addChild(Pointer child) noexcept;
        void setAttribute(const std::string& key, const std::string& value) noexcept;
        void setText(const std::string& text) noexcept;
        // -------

        // getters
        std::string getName() const noexcept;
        std::string getId() const noexcept;
        std::vector<std::string> getClasses() const noexcept;
        std::map<std::string, std::string> getAttributes() const noexcept;
        std::vector<Pointer> getChildren() const noexcept;
        std::string getText() const noexcept;
        // -------

    private:
        std::string name; ///< Name of the tag.
        std::string id; ///< ID of the tag.
        std::string text; ///< Text content of the tag.
        std::vector<std::string> classes; ///< Classes assigned to the tag.
        std::map<std::string, std::string> attributes; ///< Attributes of the tag.
        std::vector<Pointer> children; ///< Children of the tag.
        WeakPointer parent; ///< Parent of the tag.
    };

    /**
     * @brief Overload of the << operator for Tag::Pointer.
     * @param os Output stream.
     * @param tag Tag to output.
     * @return std::ostream& Reference to the output stream.
     */
    std::ostream& operator<<(std::ostream& os, const Tag::Pointer& tag);


    /**
     * @class DOM
     * @brief Manages a tree of Tag objects.
     *
     * Provides functionality to manipulate and navigate a tree of Tag objects.
     * It includes methods for printing the tree structure and accessing the root tag.
     */
    class DOM
    {
    public:
        void setRoot(Tag::Pointer root) noexcept { this->root = root; }
        Tag::Pointer getRoot() const noexcept { return root; }

        void PrintTree(const Tag::Pointer& tag, int depth = 0) const noexcept;
        void PrintTree(int depth = 0) const noexcept;

    private:
        mutable Tag::Pointer root;  ///< Root of the tag tree, mutable for const method modifications.
    };

    /**
     * @class ParserBase
     * @brief Сlass for parsers in the HiGUI system.
     *
     * Provides an interface for parsing documents and managing the resulting Tag objects.
     * Includes methods for reading documents, writing tags, finding tags, and accessing the DOM.
     */
    class ParserBase
    {
    public:
        explicit ParserBase() noexcept = default;
        virtual ~ParserBase() = default;

        virtual void read(const std::string& filename) = 0;
        bool write(const Tag::Pointer& where, const Tag& tag);

        Tag::Pointer find(const std::string& id) const;
        std::list<Tag::Pointer> findAll(const std::string& class_val) const;

        DOM& getDOM() const noexcept { return dom; }

    private:
        mutable DOM dom;  ///< DOM for managing parsed tags, mutable for const method modifications.

        static Tag::Pointer findRecursive(Tag::Pointer current, const std::string& id);
        static void findAllRecursive(Tag::Pointer current, const std::string& class_val, std::list<Tag::Pointer>& result);
    };

    /**
     * @class ParsingException
     * @brief Exception class for handling parsing errors.
     *
     * Encapsulates information about errors that occur during parsing, including the file name, line number, and error message.
     */
    class ParsingException : public std::exception
    {
    public:
        ParsingException(const std::string& message, const std::string& filename = "", int line_number = -1);

        const char* what() const noexcept override;
        const std::string& getFilename() const noexcept;
        int getLineNumber() const noexcept;

    private:
        std::string filename; ///< Name of the file where the error occurred.
        int line_number; ///< Line number of the error.
        std::string full_message; ///< Full error message.
    };

} // namespace hi::parser

#endif // HiGUI_PARSER_BASE_H