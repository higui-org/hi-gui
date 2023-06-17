#ifndef MARKUP_PARSER_H
#define MARKUP_PARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>

#include <type_traits>
#include <memory>

#include "gui_object.h"

namespace higui
{

	class MarkupParser
	{
	public:
		MarkupParser(std::string filename);
		~MarkupParser();

		template <typename T>
		void AddClass(std::string name);

	private:
		void InitObjects();

		int getLineNumber(int offset);
		bool isMarkupValid();

		std::string getTagBloc(size_t offset);
		std::string getTagName(std::string& tag_bloc);
		std::string getTagValue(std::string& tag_bloc, std::string key);

		std::string markup;
		std::unordered_map<GUIObject*, std::string*> map;
		AnyMap<std::string> classes;
	};

	template <typename T>
	void MarkupParser::AddClass(std::string name)
	{
		if (classes.contains(name))
		{
			std::cout << "Class with the same name already exists" << std::endl;
			return;
		}

		classes.insert(name, std::make_shared<T>());

		std::shared_ptr<GUIObject> obj = classes.at<std::shared_ptr<GUIObject>>(name);
		if (obj)
		{
			//using type = typename std::remove_pointer<>::type;
			//std::shared_ptr<type> value = AnyCast<std::shared_ptr<type>>(pair.second);
			//std::shared_ptr<GUIObject> obj = std::static_pointer_cast<GUIObject>(value);
			if (obj)
			{
				std::cout << obj->name << std::endl;
			}
		}
	}
}

#endif // MARKUP_PARSER_H