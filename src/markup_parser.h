#ifndef MARKUP_PARSER_H
#define MARKUP_PARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>

#include "gui_object.h"

namespace higui
{

	class MarkupParser
	{
	public:
		MarkupParser(std::string filename);
		~MarkupParser();

		//template <typename T>
		//void addClass(std::string name);

	private:
		void InitObjects();

		int getLineNumber(int offset);
		bool isMarkupValid();

		std::string getTagBloc(int offset);
		std::string getTagName(std::string& tag_bloc);
		std::string getTagValue(std::string& tag_bloc, std::string key);

		std::string markup;
		std::unordered_map<GUIObject*, std::string*> map;
		static AnyMap<std::string> classes;
	};
}

#endif // MARKUP_PARSER_H