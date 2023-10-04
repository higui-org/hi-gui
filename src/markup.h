#ifndef HI_MARKUP_H
#define HI_MARKUP_H

#include <stack>
#include <cctype>

#include "object.h"

namespace higui
{
	class Markup
	{
	public:
		Markup(const std::string& markup_file);

		void Init();

	private:
		std::string CookMarkup(const std::string& markup);

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(const std::string& tag_block);
		std::string ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name);
		std::vector<Attribute> ExtractAttributes(const std::string& tag_block);

		void ProcessOpeningTag(const std::string& tag_block, std::stack<std::shared_ptr<internal::GUIObjectBase>>& object_stack);
		void ProcessClosingTag(std::stack<std::string>& tag_stack, std::stack<std::shared_ptr<internal::GUIObjectBase>>& object_stack);

		std::string markup_raw;
		std::string markup_cooked;
		std::shared_ptr<internal::GUIObjectBase> central_object;

		friend class DOM;
	};
}

#endif // HI_MARKUP_H