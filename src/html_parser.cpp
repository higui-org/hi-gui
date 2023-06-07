#include "html_parser.h"

HTMLParser::HTMLParser(std::string filename)
{
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
		std::cout << "ERROR::HTML_PARSER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	if (CheckMarkupSyntax())
	{
		InitObjects();
	}
}

HTMLParser::~HTMLParser()
{
}

void HTMLParser::InitObjects()
{

}

int HTMLParser::getLineNumber(int offset)
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

bool HTMLParser::CheckMarkupSyntax()
{
	bool no_errors = true;
	int nest = 0;

	for (size_t i = 0; i < markup.length(); i++)
	{
		// tag block starts
		if (markup[i] == '<')
		{

			if (markup[i + 1] == '/')
			{
				nest--;
			}
			else
			{
				nest++;
				std::cout << getTagBloc(i) << std::endl;
				std::cout << getTagName(getTagBloc(i)) << std::endl;
			}
		}
	}

	if (nest != 0)
	{
		std::cout << "ERROR::HTML_PARSER::SYNTAX_ERROR::TAG_NOT_CLOSED" << std::endl;
		no_errors = false;
	}

	return no_errors;
}

std::string HTMLParser::getTagBloc(int offset)
{
	size_t bloc_start, bloc_end;
	bloc_start = bloc_end = offset;
	
	for (; markup[bloc_end] != '>'; ++bloc_end) { }
	return markup.substr(bloc_start, bloc_end - bloc_start + 1);
}

std::string HTMLParser::getTagName(std::string tag_block)
{
	bool name_starting = false;
	for (size_t i = 1; i < tag_block.length(); i++)
	{
		if (!name_starting && markup[i] != ' ')
		{
			name_starting = true;
		}
		if (markup[i] == ' ')
		{
			return tag_block.substr(1, i-1);
		}
	}
	return "none";
}

std::string HTMLParser::getTagClass(std::string tag_block)
{
	return "e";
}