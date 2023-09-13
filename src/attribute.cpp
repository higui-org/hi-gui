#include "attribute.h"

namespace higui
{
	Attribute::Attribute(const std::string& key) : key_(key) {
		auto it = AttributeValue::registry().find(key);
		if (it != AttributeValue::registry().end())
		{
			std::shared_ptr<AttributeValue> new_value = it->second();
			attr_value = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + key);
		}
	}

	Attribute::Attribute(const std::string& key, const std::string& value) : key_(key) {
		std::string type = key;
		std::string real_value = value;

		size_t colon_pos = value.find(':');
		if (colon_pos != std::string::npos)
		{
			type = value.substr(0, colon_pos);
			real_value = value.substr(colon_pos + 1);
		}

		auto it = AttributeValue::registry().find(type);
		if (it != AttributeValue::registry().end())
		{
			std::shared_ptr<AttributeValue> new_value = it->second();
			new_value->fromString(real_value);
			attr_value = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + type);
		}
	}
}