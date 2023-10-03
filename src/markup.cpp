#include "higui.h"

namespace higui
{
	Markup::Markup(const std::string& filename)
	{	
		central_object = std::make_shared<DivTag>();
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
		std::string markup = CookMarkup();
		if (!markup.empty())
		{
			throw std::runtime_error("Markup isn't valid");
		}

		std::stack<std::shared_ptr<internal::GUIObjectBase>> object_stack;
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup.length())
		{
			// Tag block starts
			std::string tag_block = ExtractTagBlock(pos);
			std::string tag_name = ExtractTagName(tag_block);

			if (tag_block[1] == '/') // Closing tag
			{
				ProcessClosingTag(tag_stack, object_stack);
			}
			else // Opening tag
			{
				tag_stack.push(tag_name);
				ProcessOpeningTag(tag_block, object_stack);
			}

			size_t closing_tag_pos = markup.find('>', pos);
			pos = closing_tag_pos + 1;
		}

		markup.clear();
		markup.shrink_to_fit();
	}

	const std::string& Markup::CookMarkup()
	{
		std::stack<std::string> tag_stack;
		size_t pos = 0;

		while (pos < markup_raw.length())
		{
			// Tag block starts
			std::string tag_block = ExtractTagBlock(pos);
			std::string tag_name = ExtractTagName(tag_block);

			size_t opening_tag_pos = markup_raw.find('<', pos);
			size_t closing_tag_pos = markup_raw.find('>', pos);

			if (closing_tag_pos == std::string::npos || closing_tag_pos < opening_tag_pos)
			{
				throw std::runtime_error("MarkupParser::syntax_error_invalid_tag");
				return false;
			}

			if (tag_block[1] == '/') // Closing tag
			{
				if (tag_stack.empty() || tag_stack.top() != tag_name)
				{
					throw std::runtime_error("MarkupParser::syntax_error_invalid_tag");
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

		if (tag_stack.empty())	// Markup is valid if all opening tags have corresponding closing tags
		{
			std::string cleaned_markup;
			bool inside_quotes = false;
			bool first_attribute = true;

			for (char c : markup_raw) {
				if (c == '"') {
					inside_quotes = !inside_quotes;
				}

				if (inside_quotes || !std::isspace(c)) {
					cleaned_markup += c;
					if (!inside_quotes && c == '=' && first_attribute) {
						cleaned_markup += ' ';
						first_attribute = false;
					}
				}
			}

			return cleaned_markup;
		}

		return "";
	}

	std::string Markup::ExtractTagBlock(size_t offset)
	{
		size_t block_pos, block_length;
		block_pos = block_length = offset;

		block_pos = markup_raw.find('<', offset);
		block_length = markup_raw.find('>', block_pos);

		if (block_pos == std::string::npos || block_length == std::string::npos)
		{
			throw std::runtime_error("MarkupParser::syntax_error_closing_tag_missing_or_mismatched");
			return "";
		}

		return markup_raw.substr(block_pos, block_length - block_pos + 1);
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
				std::vector<Attribute> attributes = ExtractAttributes(tag_block);

				for (auto& attribute : attributes)
				{
					obj->attribute.add(attribute);
				}
				// Set object as a child of the previous object
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