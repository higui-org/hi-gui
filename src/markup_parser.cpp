#include "higui.h"

namespace higui
{
	std::unordered_map<std::string, std::function<void* ()>> MarkupParser::class_factories;

	MarkupParser::MarkupParser(const std::string& filename)
	{
		central_object = new GUIObject;
		
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		file.open(filename);
		std::stringstream stream;

		stream << file.rdbuf();
		file.close();

		markup = stream.str();
	}

	MarkupParser::~MarkupParser()
	{
		delete central_object;	// delete central object
	}

	void MarkupParser::Init()
	{
		if (!isMarkupValid())
		{
			throw std::runtime_error("Markup isn't valid");
			return;
		}

		std::stack<GUIObject*> object_stack;
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup.length())
		{
			// tag block starts
			std::string tag_block = ExtractTagBlock(pos);
			std::string tag_name = ExtractTagName(tag_block);

			size_t closing_tag_pos = markup.find('>', pos);

			if (tag_block[1] == '/') // closing tag
			{
				if (!tag_stack.empty())
				{
					tag_stack.pop();
					if (!object_stack.empty())
					{
						GUIObject* obj = object_stack.top();
						object_stack.pop();
					}
				}
			}
			else // opening tag
			{
				tag_stack.push(tag_name);
				if (class_factories.find(tag_name) != class_factories.end())
				{
					GUIObject* obj = static_cast<GUIObject*>(class_factories[tag_name]());
					if (obj)
					{
						// get and set attributes to object
						std::unordered_map<std::string, std::string> attributes = ExtractAttributes(tag_block);
						for (auto& attribute : attributes)
						{
							obj->set(attribute.first, attribute.second);
						}
						// set object as child of previous object
						if (!object_stack.empty())
						{
							object_stack.top()->AddChild(obj);
							obj->parent = object_stack.top();
						}
						else
						{
							central_object->AddChild(obj);
						}
						object_stack.push(obj);
						obj->Update();
					}
				}
			}

			pos = closing_tag_pos + 1;
		}
		markup.clear();
		markup.shrink_to_fit();
	}



	bool MarkupParser::isMarkupValid()
	{
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup.length())
		{
			// tag block starts
			std::string tag_block = ExtractTagBlock(pos);
			std::string tag_name = ExtractTagName(tag_block);

			size_t opening_tag_pos = markup.find('<', pos);
			size_t closing_tag_pos = markup.find('>', pos);

			if (closing_tag_pos == std::string::npos || closing_tag_pos < opening_tag_pos)
			{
				throw std::runtime_error("MarkupParser::syntax_error_invalid_tag");
				return false;
			}

			if (tag_block[1] == '/') // closing tag
			{
				if (tag_stack.empty() || tag_stack.top() != tag_name)
				{
					throw std::runtime_error("MarkupParser::syntax_error_invalid_tag");
					return false;
				}
				tag_stack.pop();
			}
			else // opening tag
			{
				tag_stack.push(tag_name);
			}

			pos = closing_tag_pos + 1;
		}

		return tag_stack.empty(); // markup is valid if all opening tags have corresponding closing tags
	}

	std::string MarkupParser::ExtractTagBlock(size_t offset)
	{
		size_t block_pos, block_length;
		block_pos = block_length = offset;

		block_pos = markup.find('<', offset);
		block_length = markup.find('>', block_pos);

		if (block_pos == std::string::npos || block_length == std::string::npos)
		{
			throw std::runtime_error("MarkupParser::syntax_error_closing_tag_missing_or_mismatched");
			return "";
		}

		return markup.substr(block_pos, block_length - block_pos + 1);
	}

	std::string MarkupParser::ExtractTagName(std::string& tag_block)
	{
		size_t pos, end_pos;
		pos = tag_block.find('/');	   // trying to find as closing tag
		end_pos = tag_block.find('>');
		if (pos == std::string::npos || end_pos == std::string::npos)
		{
			pos = tag_block.find('<'); // trying to find as opening tag
			end_pos = tag_block.find(' ', pos);
			if (pos == std::string::npos || end_pos == std::string::npos)
				return "";
		}
		return tag_block.substr(pos + 1, end_pos - pos - 1);
	}

	std::string MarkupParser::ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name)
	{
		size_t key_pos = tag_block.find(attribute_name + "=");
		if (key_pos == std::string::npos)
			return ""; // key not found

		size_t value_pos = key_pos + attribute_name.length() + 1;
		char opening_quote = tag_block[value_pos];
		if (opening_quote != '\'' && opening_quote != '\"')
			return ""; // opening quote character not found

		size_t closing_quote_pos = tag_block.find_first_of(opening_quote, value_pos + 1);
		if (closing_quote_pos == std::string::npos)
			return ""; // closing quote character not found

		size_t value_length = closing_quote_pos - value_pos - 1;

		return tag_block.substr(value_pos + 1, value_length);
	}

	std::unordered_map<std::string, std::string> MarkupParser::ExtractAttributes(const std::string& tag_block)
	{
		std::unordered_map<std::string, std::string> attributes;

		std::istringstream iss(tag_block);
		std::string tag;
		iss >> tag;

		while (iss)
		{
			std::string attr;
			iss >> attr;

			if (!attr.empty())
			{
				std::size_t equals_pos = attr.find('=');
				if (equals_pos != std::string::npos)
				{
					std::string attribute_name = attr.substr(0, equals_pos);
					std::string attribute_value = attr.substr(equals_pos + 1);

					// remove double quotes and "greater than" sign, if they are present
					if (!attribute_value.empty())
					{
						if (attribute_value.back() == '>')
						{
							attribute_value = attribute_value.substr(0, attribute_value.length() - 1);
						}
						if (attribute_value.front() == '"' || attribute_value.front() == '\'')
						{
							attribute_value = attribute_value.substr(1, attribute_value.length() - 2);
						}
					}

					attributes[attribute_name] = attribute_value;
				}
			}
		}
		return attributes;
	}
}