#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "gui_object.h"

class HTMLParser
{
public:
	HTMLParser(std::string filename);
	~HTMLParser();

private:
	void InitObjects();

	int getLineNumber(int offset);
	bool CheckMarkupSyntax();	 // returns true if no errors detected

	std::string getTagBloc(int offset);
	std::string getTagName(std::string tag_block);
	std::string getTagClass(std::string tag_block);

	std::string markup;
	std::unordered_map<GUIObject*, std::string*> map;
};

#endif // HTMLPARSER_H