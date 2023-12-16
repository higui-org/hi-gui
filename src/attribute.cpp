#include "attribute.h"

namespace hi
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

	// AttributeContainer

	template <class AttrValue>
	AttrValue& AttributeContainer::operator()(const std::string& key = "") {
		std::string effective_key = key;
		if (effective_key.empty()) {
			for (const auto& entry : internal::AttributeValueBase::registry()) {
				if (std::dynamic_pointer_cast<Derived>(entry.second())) {
					effective_key = entry.first;
					break;
				}
			}
			if (effective_key.empty()) {
				throw std::runtime_error("Could not determine key for the given type in the registry.");
			}
		}

		std::size_t attr_index = find(effective_key);
		if (attr_index != npos) {
			if (std::shared_ptr<Derived> derived_value = std::dynamic_pointer_cast<Derived>(elements[attr_index].value_)) {
				return *derived_value;
			}
			else {
				throw std::runtime_error("Attribute value cannot be converted to the specified type.");
			}
		}
		std::shared_ptr<Derived> new_attribute = std::make_shared<Derived>();
		Attribute attr(effective_key);
		attr.value_ = new_attribute;
		elements.push_back(attr);
		return *new_attribute;
	}  // AttributeContainer operator "()"

	void AttributeContainer::add(const std::string& key) {
		elements.push_back(Attribute(key));
	}

	void AttributeContainer::add(Attribute attribute) {
		elements.push_back(attribute);
	}

	void AttributeContainer::add(const std::string& key, const std::string& value) {
		elements.push_back(Attribute(key, value));
	}

	template <typename T>
	std::enable_if_t<!(std::is_same_v<T, std::string>),
		void>
		AttributeContainer::add(const std::string& key, const T& value)
	{
		Attribute attr{ key };
		attr = value;
		elements.push_back(attr);
	}

	void AttributeContainer::remove(const std::string& key) {
		elements.erase(std::remove_if(elements.begin(), elements.end(),
			[key](const Attribute& attr) { return attr.key() == key; }), elements.end());
	}

	bool AttributeContainer::has(const std::string& key) const {
		for (const auto& attr : elements) {
			if (attr.key() == key) {
				return true;
			}
		}
		return false;
	}

	std::reference_wrapper<Attribute> AttributeContainer::attr(const std::string& key) {
		std::size_t attr_index = find(key);
		if (attr_index != npos)
		{
			return elements[attr_index];
		}
		add(key);
		return std::ref(elements.back());
	}

	std::optional<std::reference_wrapper<Attribute>> 
		AttributeContainer::attr_if(const std::string& key, const std::function<bool(const Attribute& attribute)>& func)
	{
		for (auto& a : elements) {
			if (func(a)) return a;
		}
	}

	std::reference_wrapper<Attribute> AttributeContainer::attr_if_exist(const std::string& key) {
		std::size_t attr_index = find(key);
		if (attr_index != npos)
		{
			return elements[attr_index];
		}
		throw std::runtime_error("Attribute key \"" + key + "\" not found");
	}

	std::size_t AttributeContainer::find(const std::string& key) const {
		for (std::size_t i = 0; i < elements.size(); ++i) {
			if (elements[i].key() == key) {
				return i;
			}
		}
		return npos;
	}

	class AttributeContainer::Iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = const Attribute;
		using reference = const Attribute&;
		using pointer = const Attribute*;
		using difference_type = std::ptrdiff_t;

		reference operator*() const {
			return *it;
		}

		pointer operator->() const {
			return &(*it);
		}

		Iterator& operator++() {
			++it;
			return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			++it;
			return temp;
		}

		bool operator==(const Iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const Iterator& other) const {
			return it != other.it;
		}

	private:
		std::vector<Attribute>::const_iterator it;
	};
}