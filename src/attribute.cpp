#include "attribute.h"

namespace higui
{
	Attribute::Attribute(const std::string& key) : key_(key) {
		auto it = AttributeValueBase::registry().find(key_);
		if (it != AttributeValueBase::registry().end())
		{
			std::shared_ptr<AttributeValueBase> new_value = it->second();
			value_ = new_value;
		}
		else {
			value_ = nullptr;
		}
	}

	Attribute::Attribute(const std::string& key, const std::string& type_value) : key_(key) {
		this->operator=(type_value);
	}

	Attribute& Attribute::operator=(const std::string& type_value)
	{
		std::string type{"str"};
		std::string new_value{type_value};
		size_t colon_pos{ type_value.find(':') };
		if (colon_pos != std::string::npos)
		{
			type = type_value.substr(0, colon_pos);
			new_value = type_value.substr(colon_pos + 1);
			new_value.erase(0, new_value.find_first_not_of(' '));
		}

		auto it_key = AttributeValueBase::registry().find(key_);
		if (it_key == AttributeValueBase::registry().end())		// not found
		{
			auto it_type = AttributeValueBase::registry().find(type);	// find by ':'
			if (it_type != AttributeValueBase::registry().end())
			{
				value_ = it_type->second();

				auto RegisterDerived = [this]() -> std::shared_ptr<AttributeValueBase> {	// register new key
					return value_->instance();
				};

				AttributeValueBase::registry()[key_] = RegisterDerived;
			}
		}
		else
		{
			value_ = it_key->second();		// just create
		}
		value_->from_str(new_value);
		return *this;
	}

	/*
	void Attribute::setKey(const std::string& key)
	{
		key_ = key;
		auto it = AttributeValueBase::registry().find(key_);
		if (it != AttributeValueBase::registry().end())
		{
			std::shared_ptr<AttributeValueBase> new_value = it->second();
			value_ = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + key_);
		}
	}
	*/

	std::ostream& operator<<(std::ostream& os, const Attribute& obj)
	{
		os << "key: " << obj.key() << "\tvalue: ";
		if (std::dynamic_pointer_cast<attribute::String>(obj.value_)) {
			os << "\"" << obj.value_->to_str() << "\"";
		}
		else {
			os << obj.value_->to_str();
		}
		return os;
	}
}