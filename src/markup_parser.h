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
		MarkupParser(const std::string& markup_file);
		~MarkupParser();

		template <typename T>
		static void RegisterClass(const std::string& name);
		void Init();

	private:
		bool isMarkupValid();

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(std::string& tag_block);
		std::string ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name);
		std::unordered_map<std::string, std::string> ExtractAttributes(const std::string& tag_block);

		std::string markup;
		GUIObject* central_object;

		static std::unordered_map<std::string, std::function<void* ()>> class_factories;

		friend class DOM;
	};

	template <typename T>
	void MarkupParser::RegisterClass(const std::string& name)
	{
		if (class_factories.find(name) != class_factories.end())
		{
			throw std::runtime_error("MarkupParser::the_class_being_registered_already_exists");
			return;
		}
		class_factories[name] = []() { return new T(); };
	}
}

#endif // MARKUP_PARSER_H