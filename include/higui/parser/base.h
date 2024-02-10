/**
 * @file Parser.h
 * @brief Header file defining the base classes for the HiGUI parser module.
 *
 * This file contains the class definitions for the HiGUI project's parser module.
 * It includes the definitions for Tag, DOM, ParserBase, and ParsingException classes,
 * which are integral to the HiGUI parsing system. These classes provide the
 * framework for parsing, managing, and handling exceptions for HTML-like structured data,
 * facilitating the construction and manipulation of a document object model (DOM).
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
     * id, classes, children, and attributes. It provides methods to manipulate
     * these properties and manage the hierarchy of tags within a parsed document.
     * Tags can represent elements similar to those in HTML, facilitating the
     * representation and manipulation of structured data.
     */
    class Tag
    {
    public:
        using Pointer = std::shared_ptr<Tag>;
        using WeakPointer = std::weak_ptr<Tag>;

        /**
         * @brief Construct a new Tag object with a specified name.
         * @param name The name of the tag.
         */
        explicit Tag(const std::string& name);

        // Setters
        /**
         * @brief Sets the name of the tag.
         * @param name New name for the tag.
         */
        void setName(const std::string& name) noexcept;

        /**
         * @brief Sets the ID of the tag.
         * @param id New ID for the tag.
         */
        void setId(const std::string& id) noexcept;

        /**
         * @brief Sets the class of the tag.
         * @param class_val New class value for the tag.
         */
        void setClass(const std::string& class_val) noexcept;

        /**
         * @brief Adds a child tag to this tag.
         * @param child Pointer to the child tag to be added.
         */
        void addChild(Pointer child) noexcept;

        /**
         * @brief Sets an attribute for the tag.
         * @param key Attribute name.
         * @param value Attribute value.
         */
        void setAttribute(const std::string& key, const std::string& value) noexcept;

        /**
         * @brief Sets the text content for the tag.
         * @param text Text to be set for the tag.
         */
        void setText(const std::string& text) noexcept;
        // -------

        // Getters
        /**
         * @brief Gets the name of the tag.
         * @return Name of the tag.
         */
        std::string getName() const noexcept;

        /**
         * @brief Gets the ID of the tag.
         * @return ID of the tag.
         */
        std::string getId() const noexcept;

        /**
         * @brief Gets the classes assigned to the tag.
         * @return Vector of class names.
         */
        std::vector<std::string> getClasses() const noexcept;

        /**
         * @brief Gets the attributes of the tag.
         * @return Map of attribute names to values.
         */
        std::map<std::string, std::string> getAttributes() const noexcept;

        /**
         * @brief Gets the children of the tag.
         * @return Vector of pointers to the child tags.
         */
        std::vector<Pointer> getChildren() const noexcept;

        /**
         * @brief Gets the text content of the tag.
         * @return Text content of the tag.
         */
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
     * @brief Overload of the << operator for Tag::Pointer to support easy printing.
     * @param os Output stream to which the tag information will be written.
     * @param tag Tag to be printed.
     * @return std::ostream& Reference to the modified output stream.
     */
    std::ostream& operator<<(std::ostream& os, const Tag::Pointer& tag);


    /**
 * @class DOM
 * @brief Manages a tree of Tag objects.
 *
 * This class provides functionality to manipulate and navigate a tree of Tag objects,
 * representing the structured hierarchy of a parsed document. It supports operations
 * such as setting the root tag, printing the structure of the tag tree, and accessing
 * the root tag of the document.
 */
    class DOM
    {
    public:
        /**
         * @brief Sets the root tag of the document.
         * @param root The root tag to be set for the document.
         */
        void setRoot(Tag::Pointer root) noexcept { this->root = root; }

        /**
         * @brief Retrieves the root tag of the document.
         * @return The root tag of the document.
         */
        Tag::Pointer getRoot() const noexcept { return root; };

        /**
         * @brief Prints the hierarchical tree structure of tags starting from a specified tag.
         * @param tag The starting tag from which the tree will be printed.
         * @param depth The initial depth for indentation (used for recursive calls).
         */
        void PrintTree(const Tag::Pointer& tag, int depth = 0) const noexcept;

        /**
         * @brief Prints the hierarchical tree structure of the entire document.
         * @param depth The initial depth for indentation (used for recursive calls).
         */
        void PrintTree(int depth = 0) const noexcept;

    private:
        mutable Tag::Pointer root; ///< The root of the tag tree, mutable to allow modification in const methods.
    };


    /**
 * @class ParserBase
 * @brief Abstract base class for parsers in the HiGUI system.
 *
 * This class provides an interface for parsing documents into a structured format
 * and managing the resulting Tag objects within a DOM. It defines the core
 * functionality required for reading documents, writing tags, finding tags, and
 * accessing the DOM. Implementations should provide specific parsing logic.
 */
    class ParserBase
    {
    public:
        ParserBase() noexcept = default;
        virtual ~ParserBase() = default;

        /**
         * @brief Reads and parses a document from the specified file.
         * @param filename The path of the file to be parsed.
         */
        virtual void read(const std::string& filename) = 0;

        /**
         * @brief Writes a new tag into a specified location within the document.
         * @param where A pointer to the tag under which the new tag will be added.
         * @param tag The tag to be added to the document.
         * @return true if the tag was successfully added, false otherwise.
         */
        bool write(const Tag::Pointer& where, const Tag& tag);

        /**
         * @brief Finds a tag by its unique ID.
         * @param id The unique ID of the tag to find.
         * @return A pointer to the found tag, or nullptr if not found.
         */
        Tag::Pointer find(const std::string& id) const;

        /**
         * @brief Finds all tags that match a given class value.
         * @param class_val The class value to match.
         * @return A list of pointers to the matching tags.
         */
        std::list<Tag::Pointer> findAll(const std::string& class_val) const;

        /**
         * @brief Gets the DOM object managed by the parser.
         * @return A reference to the DOM object.
         */
        DOM& getDOM() const noexcept { return dom; }

    private:
        mutable DOM dom; ///< The DOM for managing parsed tags, mutable for const method modifications.

        static Tag::Pointer findRecursive(Tag::Pointer current, const std::string& id);
        static void findAllRecursive(Tag::Pointer current, const std::string& class_val, std::list<Tag::Pointer>& result);
    };


    /**
     * @class ParsingException
     * @brief Exception class for handling parsing errors in the HiGUI system.
     *
     * This class encapsulates information about errors that occur during the parsing process,
     * including the file name, line number, and an error message. It extends the standard
     * exception class to provide detailed error information which can be used for debugging
     * and error reporting.
     */
    class ParsingException : public std::exception
    {
    public:
        /**
         * @brief Constructs a new ParsingException with a specified error message, file name, and line number.
         * @param message The error message describing what went wrong.
         * @param filename (Optional) The name of the file where the error occurred.
         * @param line_number (Optional) The line number in the file where the error was detected.
         */
        ParsingException(const std::string& message, const std::string& line = "", const std::string& filename = "", int line_number = -1);

        /**
         * @brief Provides the error message including file name and line number if available.
         * @return The full error message as a C-style string.
         */
        const char* what() const noexcept override;

        /**
         * @brief Gets the name of the file where the error occurred.
         * @return The file name as a standard string.
         */
        const std::string& getFilename() const noexcept;

        /**
         * @brief Gets the line number where the error was detected.
         * @return The line number.
         */
        int getLineNumber() const noexcept;

    private:
        std::string filename; ///< The name of the file where the error occurred.
        std::string line; ///< The markup line.
        int line_number; ///< The line number at which the error was detected.
        std::string full_message; ///< The full error message.
    };


} // namespace hi::parser

#endif // HiGUI_PARSER_BASE_H