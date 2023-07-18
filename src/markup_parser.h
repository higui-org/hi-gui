#ifndef MARKUP_PARSER_H
#define MARKUP_PARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <functional>
#include <memory>

#include "gui_object.h"

namespace higui
{
	class MarkupParser
	{
	public:
		MarkupParser(std::string markup_file);
		~MarkupParser();

		template <typename T>
		void RegisterClass(std::string name);
		void Init();

	private:
		void InitObjects();

		bool isMarkupValid();

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(std::string& tag_block);
		std::string ExtractAttributeValue(std::string& tag_block, std::string& attribute_name);
		std::unordered_map<std::string, std::string> ExtractAttributes(std::string& tag_block);

		std::string markup;
		std::unordered_map<std::string, std::function<void*()>> class_factories;
		std::unordered_map<std::string, void*> instances;
		GUIObject* central_object;

		friend class DOM;
	};

    template <typename T>
    void MarkupParser::RegisterClass(std::string name)
    {
		if (class_factories.find(name) != class_factories.end())
		{
#ifdef HIGUI_DEBUG_MODE
			std::cout << "WARNING::MARKUP_PARSER::THE_CLASS_BEING_REGISTERED_ALREADY_EXISTS" << std::endl;
#endif
			return;
		}
		class_factories[name] = []() { return new T(); };
		GUIObject* obj = static_cast<GUIObject*>(class_factories[name]());
		instances[name] = obj;
    }
}

#endif // MARKUP_PARSER_H