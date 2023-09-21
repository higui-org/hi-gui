#ifndef HI_MARKUP_H
#define HI_MARKUP_H

#include <stack>

#include "object.h"

namespace higui
{
	class Markup
	{
	public:
		Markup(const std::string& markup_file);

		void Init();

	private:
		bool isMarkupValid();

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(const std::string& tag_block);
		std::string ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name);
		std::unordered_map<std::string, std::string> ExtractAttributes(const std::string& tag_block);

		std::string markup;
		std::shared_ptr<GUIObject> central_object;

		friend class DOM;
	};
}

#endif // HI_MARKUP_H