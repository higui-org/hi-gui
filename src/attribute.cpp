#include "attribute.h"

namespace higui
{
	Attribute::Attribute(const std::string& key, const std::string& type_tag) : key_(key) {
		std::string type = key;
		std::string value = type_tag;

		size_t colon_pos = type_tag.find(':');
		if (colon_pos != std::string::npos)
		{
			type = type_tag.substr(0, colon_pos);
			value = type_tag.substr(colon_pos + 1);
		}

		auto it = AttributeTag::registry().find(type);
		if (it != AttributeTag::registry().end())
		{
			std::shared_ptr<AttributeTag> new_value = it->second();
			new_value->fromTag(value);
			attr_value = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + type);
		}
	}
}