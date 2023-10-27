#ifndef HI_ATTRIBUTE_H
#define HI_ATTRIBUTE_H

#include "hitypes.h"

namespace higui
{
	using namespace internal;

	class Attribute 
	{
	public:
		Attribute(const std::string& key = "str");
		Attribute(const std::string& key, const std::string& new_value);

		// for std::shared_ptr
		template <class T>
		std::enable_if_t<std::is_same<T, std::shared_ptr<class std::decay_t<T>::element_type>>::value,
			Attribute&>
			operator=(T&& new_value)
		{
			value_ = std::forward<T>(new_value);
			return *this;
		}

		// T is std::string 
		Attribute& operator=(const std::string& type_value);

		// for cloning
		template <class T>
		std::enable_if_t<!std::is_pointer_v<T> && std::is_base_of<AttributeValueBase, std::decay_t<T>>::value, Attribute&>
			operator=(const T& new_value)
		{
			value_ = std::make_shared<class std::decay<T>::type>(new_value);
			return *this;
		}

		template <class T>
		std::enable_if_t<
			std::is_same_v<T, int> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, double>,
			Attribute&>
			operator=(T new_value)
		{
			if (typeid(T) == typeid(int))
			{
				value_ = std::make_shared<attribute::Int>(static_cast<int>(new_value));
			}
			else if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
			{
				value_ = std::make_shared<attribute::Float>(static_cast<float>(new_value));
			}
			else
			{
				value_ = std::make_shared<attribute::String>();
				value_->from_str(std::to_string(new_value));
			}
			return *this;
		}

		//void setKey(const std::string& key);

		std::string key() const { return key_; }

		template <class ValueClass>
		ValueClass& value() 
		{ 
			ValueClass* v = dynamic_cast<ValueClass*>(value_.get());
			return *v; 
		}

		friend std::ostream& operator<<(std::ostream& os, const Attribute& obj);

		

	private:
		std::shared_ptr<AttributeValueBase> value_;
		std::string key_;

		friend class AttributeContainer;
	};

	class AttributeContainer {
	public:
		void add(const std::string& key) {
			attributes_.push_back(Attribute(key));
		}

		void add(Attribute attribute) {
			attributes_.push_back(attribute);
		}

		template <typename T>
		std::enable_if_t<!std::is_same_v<T, std::string>, 
		void> add(const std::string& key, const T& value)
		{
			Attribute attr{ key };
			attr = value;
			attributes_.push_back(attr);
		}

		void add(const std::string& key, const std::string& value) {
			attributes_.push_back(Attribute(key, value));
		}

		void remove(const std::string& key) {
			attributes_.erase(std::remove_if(attributes_.begin(), attributes_.end(),
				[key](const Attribute& attr) { return attr.key() == key; }), attributes_.end());
		}

		bool has(const std::string& key) const {
			for (const auto& attr : attributes_) {
				if (attr.key() == key) {
					return true;
				}
			}
			return false;
		}
		
		Attribute& operator[](const std::string& key) {
			std::size_t attr_index = find(key);
			if (attr_index != npos)
			{
				return attributes_[attr_index];
			}
			add(key);
			return attributes_.back();
		}

		template <class Derived>
		Derived& value(const std::string& key = "") {
			std::string effective_key = key;
			if (effective_key.empty()) {
				for (const auto& entry : AttributeValueBase::registry()) {
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
				if (std::shared_ptr<Derived> derived_value = std::dynamic_pointer_cast<Derived>(attributes_[attr_index].value_)) {
					return *derived_value;
				}
				else {
					throw std::runtime_error("Attribute value cannot be converted to the specified type.");
				}
			}
			std::shared_ptr<Derived> new_attribute = std::make_shared<Derived>();
			Attribute attr(effective_key);
			attr.value_ = new_attribute;
			attributes_.push_back(attr);
			return *new_attribute;
		}

		Attribute& get(const std::string& key) {
			std::size_t attr_index = find(key);
			if (attr_index != npos)
			{
				return attributes_[attr_index];
			}
			throw std::runtime_error("Attribute key \"" + key + "\" not found");
		}

		std::size_t find(const std::string& key) const {
			for (std::size_t i = 0; i < attributes_.size(); ++i) {
				if (attributes_[i].key() == key) {
					return i;
				}
			}
			return npos;
		}

		class Iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = const Attribute;
			using reference = const Attribute&;
			using pointer = const Attribute*;
			using difference_type = std::ptrdiff_t;

			Iterator(std::vector<Attribute>::const_iterator it) : it_(it) {}

			reference operator*() const {
				return *it_;
			}

			pointer operator->() const {
				return &(*it_);
			}

			Iterator& operator++() {
				++it_;
				return *this;
			}

			Iterator operator++(int) {
				Iterator temp = *this;
				++it_;
				return temp;
			}

			bool operator==(const Iterator& other) const {
				return it_ == other.it_;
			}

			bool operator!=(const Iterator& other) const {
				return it_ != other.it_;
			}

		private:
			std::vector<Attribute>::const_iterator it_;
		};

		Iterator begin() const {
			return Iterator(attributes_.begin());
		}

		Iterator end() const {
			return Iterator(attributes_.end());
		}

	private:
		std::vector<Attribute> attributes_;
		static const std::size_t npos = -1;
	};
}

#endif // HI_ATTRIBUTE_H