#ifndef Hi_MARKUP_H
#define Hi_MARKUP_H

#include <stack>
#include <cctype>

#include "object.h"

namespace hi
{
	// this class a shit, so will be rewritten in the future
	class Markup
	{
	public:
		Markup(const std::string& markup_file);

		GUIObject GetCookedStuffInOneObject();

	private:
		std::string CookMarkup(const std::string& markup);

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(const std::string& tag_block);
		std::string ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name);
		std::vector<Attribute> ExtractAttributes(const std::string& tag_block);

		void ProcessOpeningTag(const std::string& tag_block, std::stack<Widget>& object_stack);
		void ProcessClosingTag(std::stack<std::string>& tag_stack, std::stack<Widget>& object_stack);

		std::string markup_raw;
		std::string markup_cooked;
		Widget cooked_stuff;

		friend class DOM;
	};
}

#endif // Hi_MARKUP_H