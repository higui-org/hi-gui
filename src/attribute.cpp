#include "attribute.h"

namespace higui
{
	void attribute::Alignment::from_str(const std::string& tag) {
		std::vector<std::string> splitted = SplitBySpace(tag);

		std::string dock_str, ratio_str;
		try
		{
			dock_str = splitted.at(0);
		}
		catch (std::out_of_range)
		{
			dock_str = "none";
		}

		try
		{
			ratio_str = splitted.at(1);
		}
		catch (std::out_of_range)
		{
			ratio_str = "25%";
		}

		if (!dock_str.empty())
		{
			if (dock_str == "left")
			{
				pos = Align::Left;
			}
			else if (dock_str == "top")
			{
				pos = Align::Top;
			}
			else if (dock_str == "right")
			{
				pos = Align::Right;
			}
			else if (dock_str == "bottom")
			{
				pos = Align::Bottom;
			}
		}
		if (!ratio_str.empty())
		{
			ratio = internal::ToNormalizedFloat(ratio_str);
		}

	}

	std::string attribute::Alignment::to_str() {
		switch (pos) {
		case Align::Top:
			return "top " + std::to_string(ratio);
		case Align::Left:
			return "left " + std::to_string(ratio);
		case Align::Bottom:
			return "bottom " + std::to_string(ratio);
		case Align::Right:
			return "right " + std::to_string(ratio);
		default:
			return "none " + std::to_string(ratio);
		}
	}

	std::string attribute::Bool::to_str() {
		return value ? "true" : "false";
	}

	void attribute::Bool::from_str(const std::string& new_value)
	{
		if (new_value.empty() | new_value == "false")
		{
			value = false;
		}
		value = true;
	}

	Attribute::Attribute(const std::string& key) : key_(key) {
		using ValueBase = internal::AttributeValueBase;
		auto it = ValueBase::registry().find(key_);

		value_ = it != ValueBase::registry().end() ? it->second() : nullptr;
	}

	Attribute::Attribute(const std::string& key, const std::string& type_value) : key_(key) {
		this->operator=(type_value);
	}

	Attribute& Attribute::operator=(const std::string& type_value)
	{
		using ValueBase = internal::AttributeValueBase;

		std::string type{"str"};
		std::string new_value{type_value};
		size_t colon_pos{ type_value.find(':') };

		if (colon_pos != std::string::npos)
		{
			type = type_value.substr(0, colon_pos);
			new_value = type_value.substr(colon_pos + 1);
			new_value.erase(0, new_value.find_first_not_of(' '));
		}

		auto it_key = ValueBase::registry().find(key_);
		if (it_key == ValueBase::registry().end())		// not found
		{
			auto it_type = ValueBase::registry().find(type);	// find by ':'
			if (it_type != ValueBase::registry().end())
			{
				value_ = it_type->second();

				auto RegisterDerived = [this]() -> std::shared_ptr<ValueBase> {	// register new key
					return value_->instance();
				};

				ValueBase::registry()[key_] = RegisterDerived;
			}
		}
		else
		{
			value_ = it_key->second();		// just create
		}
		value_->from_str(new_value);
		return *this;
	}

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