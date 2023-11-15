#include "higui.h"

namespace higui
{
	Markup::Markup(const std::string& filename)
	{	
		central_object = std::make_shared<tag::Object>();
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		file.open(filename);
		std::stringstream stream;

		stream << file.rdbuf();
		file.close();

		markup_raw = stream.str();
	}

	void Markup::Init()
	{
		markup_cooked = CookMarkup(markup_raw);
		if (markup_cooked.empty())
		{
			throw std::runtime_error("Invalid markup");
		}

		std::stack<std::shared_ptr<internal::GUIObjectBase>> object_stack;
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup_cooked.length())
		{
			// Tag block starts
			std::string tag_block = ExtractTagBlock(pos);

			if (tag_block[1] == '/') // Closing tag
			{
				ProcessClosingTag(tag_stack, object_stack);
			}
			else // Opening tag
			{
				tag_stack.push(ExtractTagName(tag_block));
				ProcessOpeningTag(tag_block, object_stack);
			}

			size_t closing_tag_pos = markup_cooked.find('>', pos);
			pos = closing_tag_pos + 1;
		}

		markup_cooked.clear();
		markup_cooked.shrink_to_fit();
	}

	std::string Markup::CookMarkup(const std::string& markup)
	{
		std::string cooked{};
		bool inside_quotes = false;
		bool tag_name_found = false;
		bool add_space = false;

		for (char c : markup) {
			if (!cooked.empty())
			{
				if (cooked.back() == '<')
				{
					tag_name_found = false;
				}

				if (cooked.back() != '<' && c == ' ' && !tag_name_found)
				{
					add_space = true;
					tag_name_found = true;
				}
			}

			if (c == '"' || c == '\'')
			{
				inside_quotes = !inside_quotes;
				if (!inside_quotes)
				{
					add_space = true;
				}
			}
			if (c != ' ' || inside_quotes)
			{
				cooked += c;
			}
			if (add_space)
			{
				cooked += ' ';
				add_space = false;
			}
		}

		return cooked;
	}

	std::string Markup::ExtractTagBlock(size_t offset)
	{
		size_t block_pos, block_length;
		block_pos = block_length = offset;

		block_pos = markup_cooked.find('<', offset);
		block_length = markup_cooked.find('>', block_pos);

		if (block_pos == std::string::npos || block_length == std::string::npos)
		{
			throw std::runtime_error("MarkupParser::syntax_error_closing_tag_missing_or_mismatched");
		}

		return markup_cooked.substr(block_pos, block_length - block_pos + 1);
	}

	std::string Markup::ExtractTagName(const std::string& tag_block)
	{
		size_t pos, end_pos;
		pos = tag_block.find('/');	   // find as closing tag
		end_pos = tag_block.find('>');
		if (pos == std::string::npos || end_pos == std::string::npos)
		{
			pos = tag_block.find('<'); // find as opening tag
			end_pos = tag_block.find(' ', pos);
			if (pos == std::string::npos || end_pos == std::string::npos)
				return "";
		}
		return tag_block.substr(pos + 1, end_pos - pos - 1);
	}

	std::string Markup::ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name)
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

	std::vector<Attribute> Markup::ExtractAttributes(const std::string& tag_block) {
		std::vector<Attribute> attributes;

		size_t attr_start = tag_block.find_first_of(' ', 1) + 1;

		while (attr_start < tag_block.length()) {
			size_t attr_delim_pos = tag_block.find_first_of('=', attr_start);

			// Find a position that closes a double or single quotation mark
			size_t quote_start = tag_block.find_first_of("\"'", attr_delim_pos);
			if (quote_start == std::string::npos) {
				break; // If there are no closing quotes, exit
			}

			char quote_char = tag_block[quote_start];
			size_t quote_end = tag_block.find(quote_char, quote_start + 1);
			if (quote_end == std::string::npos) {
				break; // If there are no closing quotes, exit
			}

			// Extract the attribute name and its value
			std::string attribute_name = tag_block.substr(attr_start, attr_delim_pos - attr_start);
			std::string attribute_value = tag_block.substr(quote_start + 1, quote_end - quote_start - 1);

			attributes.push_back(Attribute(attribute_name, attribute_value));

			// Go to the next attribute
			attr_start = tag_block.find_first_not_of(" \t", quote_end + 1);
		}

		return attributes;
	}


	void Markup::ProcessOpeningTag(const std::string& tag_block, std::stack<std::shared_ptr<internal::GUIObjectBase>>& object_stack)
	{
		std::string tag_name = ExtractTagName(tag_block);

		auto it = internal::GUIObjectBase::registry().find(tag_name);
		if (it != internal::GUIObjectBase::registry().end())
		{
			std::shared_ptr<internal::GUIObjectBase> obj = static_cast<std::shared_ptr<internal::GUIObjectBase>>(it->second());
			if (obj)
			{
				// Extract attributes from tag block and add to the object
				obj->attribute_container = ExtractAttributes(tag_block);

				// Set object as a child of the previous object
				if (!object_stack.empty() && !obj->attribute_container.has("adopted") | obj->attr<attribute::Bool>("adopted").value == false)
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

	void Markup::ProcessClosingTag(std::stack<std::string>& tag_stack, std::stack<std::shared_ptr<internal::GUIObjectBase>>& object_stack)
	{
		if (!tag_stack.empty())
		{
			tag_stack.pop();
			if (!object_stack.empty())
			{
				object_stack.pop();
			}
		}
	}
}