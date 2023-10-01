#include "attribute.h"

namespace higui
{
	Attribute::Attribute(const std::string& key) {
		key_ = key;
		auto it = internal::AttributeValueBase::registry().find(key_);
		if (it != internal::AttributeValueBase::registry().end())
		{
			std::shared_ptr<internal::AttributeValueBase> new_value = it->second();
			value = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + key_);
		}
	}

	Attribute::Attribute(const std::string& key, const std::string& type_tag) : key_(key) {
		std::string type = key;
		std::string new_value = type_tag;

		size_t colon_pos = type_tag.find(':');
		if (colon_pos != std::string::npos)
		{
			type = type_tag.substr(0, colon_pos);
			new_value = type_tag.substr(colon_pos + 1);
		}

		auto it = internal::AttributeValueBase::registry().find(type);
		if (it != internal::AttributeValueBase::registry().end())
		{
			std::shared_ptr<internal::AttributeValueBase> value_attr = it->second();
			value_attr->fromString(new_value);
			value = value_attr;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + type);
		}
	}
}