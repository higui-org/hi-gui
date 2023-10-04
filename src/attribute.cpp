#include "attribute.h"

namespace higui
{
	Attribute::Attribute(const std::string& key) {
		key_ = key;
		auto it = internal::attr::ValueBase::registry().find(key_);
		if (it != internal::attr::ValueBase::registry().end())
		{
			std::shared_ptr<internal::attr::ValueBase> new_value = it->second();
			value = new_value;
		}
		else {
			value = nullptr;
		}
	}

	Attribute::Attribute(const std::string& key, const std::string& type_value) : key_(key) {
		key_ = key;
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

		auto it_key = internal::attr::ValueBase::registry().find(key_);
		if (it_key == internal::attr::ValueBase::registry().end())		// not found
		{
			auto it_type = internal::attr::ValueBase::registry().find(type);	// find by ':'
			if (it_type != internal::attr::ValueBase::registry().end())
			{
				value = it_type->second();

				auto RegisterDerived = [this]() -> std::shared_ptr<internal::attr::ValueBase> {	// register new key
					return value->instance();
				};

				internal::attr::ValueBase::registry()[key_] = RegisterDerived;
			}
		}
		else
		{
			value = it_key->second();		// just create
		}
		value->fromString(new_value);
		return *this;
	}

	void Attribute::setKey(const std::string& key)
	{
		key_ = key;
		auto it = internal::attr::ValueBase::registry().find(key_);
		if (it != internal::attr::ValueBase::registry().end())
		{
			std::shared_ptr<internal::attr::ValueBase> new_value = it->second();
			value = new_value;
		}
		else {
			throw std::runtime_error("Invalid Attribute key: " + key_);
		}
	}

	std::ostream& operator<<(std::ostream& os, const Attribute& obj)
	{
		os << "key: " << obj.key() << "\tvalue: ";
		if (std::dynamic_pointer_cast<attr::String>(obj.value)) {
			os << "\"" << obj.value->toString() << "\"";
		}
		else {
			os << obj.value->toString();
		}
		return os;
	}
}