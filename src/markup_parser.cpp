#include "higui.h"

namespace higui
{

	MarkupParser::MarkupParser(std::string filename)
	{
		central_object = new GUIObject;
		
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(filename);
			std::stringstream stream;

			stream << file.rdbuf();
			file.close();

			markup = stream.str();
		}
		catch (std::ifstream::failure e)
		{
#ifdef HIGUI_DEBUG_MODE
			std::cout << "ERROR::MARKUP_PARSER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
#endif
		}
	}

	MarkupParser::~MarkupParser()
	{
		delete central_object;	// delete central object

		for (auto instance : instances)		// delete all instances
		{
			delete static_cast<GUIObject*>(instance.second);
		}
	}

	void MarkupParser::Init()
	{
		if (isMarkupValid())
		{
			InitObjects();
		}
	}

	void MarkupParser::InitObjects()
	{
		std::stack<GUIObject*> object_stack;
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup.length())
		{
			// tag block starts
			std::string tag_bloc = getTagBloc(pos);
			std::string tag_name = getTagName(tag_bloc);

			size_t closing_tag_pos = markup.find('>', pos);

			if (tag_bloc[1] == '/') // Closing tag
			{
				if (!tag_stack.empty())
				{
					tag_stack.pop();
					if (!object_stack.empty())
					{
						GUIObject* obj = object_stack.top();
						object_stack.pop();
						std::cout << "Name: " << obj->name << std::endl;
					}
				}
			}
			else // Opening tag
			{
				tag_stack.push(tag_name);
				if (class_factories.find(tag_name) != class_factories.end())
				{
					GUIObject* obj = static_cast<GUIObject*>(class_factories[tag_name]());
					if (obj)
					{
						if (!object_stack.empty())
						{
							object_stack.top()->AddChild(obj);
						}
						else
						{
							central_object->AddChild(obj);
						}
						object_stack.push(obj);
					}
				}
			}

			pos = closing_tag_pos + 1;
		}
		markup.clear();
		markup.shrink_to_fit();
		std::cout << "init objects done" << std::endl;
	}

	int MarkupParser::getLineNumber(int offset)
	{
		int line = 0;
		if (offset < markup.length())
		{
			for (size_t i = 0; i < offset; i++)
			{
				if (markup[i] == '\n') line++;
			}
			return line;
		}
		return -1;
	}

	bool MarkupParser::isMarkupValid()
	{
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup.length())
		{
			// tag block starts
			std::string tag_bloc = getTagBloc(pos);
			std::string tag_name = getTagName(tag_bloc);

			size_t opening_tag_pos = markup.find('<', pos);
			size_t closing_tag_pos = markup.find('>', pos);

			if (closing_tag_pos == std::string::npos || closing_tag_pos < opening_tag_pos)
			{
#ifdef HIGUI_DEBUG_MODE
				std::cout << "ERROR::MARKUP_PARSER::SYNTAX_ERROR::INVALID_TAG" << std::endl;
#endif
				return false;
			}

			if (tag_bloc[1] == '/') // Closing tag
			{
				if (tag_stack.empty() || tag_stack.top() != tag_name)
				{
#ifdef HIGUI_DEBUG_MODE
					std::cout << "ERROR::MARKUP_PARSER::SYNTAX_ERROR::INVALID_TAG" << std::endl;
#endif
					return false;
				}
				tag_stack.pop();
			}
			else // Opening tag
			{
				tag_stack.push(tag_name);
			}

			pos = closing_tag_pos + 1;
		}

		return tag_stack.empty(); // Markup is valid if all opening tags have corresponding closing tags
	}

	std::string MarkupParser::getTagBloc(size_t offset)
	{
		size_t bloc_pos, bloc_length;
		bloc_pos = bloc_length = offset;

		bloc_pos = markup.find('<', offset);
		bloc_length = markup.find('>', bloc_pos);

		if (bloc_pos == std::string::npos || bloc_length == std::string::npos)
		{
#ifdef HIGUI_DEBUG_MODE
			std::cout << "ERROR::MARKUP_PARSER::SYNTAX_ERROR::CLOSING_TAG_MISSING_OR_MISMATCHED" << std::endl;
#endif
			return "none";
		}

		return markup.substr(bloc_pos, bloc_length - bloc_pos + 1);
	}

	std::string MarkupParser::getTagName(std::string& tag_bloc)
	{
		size_t pos, end_pos;
		pos = tag_bloc.find('/');							// trying to find as closing tag
		end_pos = tag_bloc.find('>');
		if (pos == std::string::npos || end_pos == std::string::npos)
		{
			pos = tag_bloc.find('<');						// trying to find as opening tag
			end_pos = tag_bloc.find(' ', pos);
			if (pos == std::string::npos || end_pos == std::string::npos)
				return "none";
		}
		return tag_bloc.substr(pos + 1, end_pos - pos - 1);
	}

	std::string MarkupParser::getTagValue(std::string& tag_bloc, std::string key)
	{
		size_t key_pos = tag_bloc.find(key + "=");
		if (key_pos == std::string::npos)
			return ""; // key not found

		size_t value_pos = key_pos + key.length() + 1;
		size_t closing_pos = tag_bloc.find_first_of(" >", value_pos);
		if (closing_pos == std::string::npos)
			return ""; // value closing character not found

		size_t value_length = closing_pos - value_pos;
		return tag_bloc.substr(value_pos, value_length);
	}

}