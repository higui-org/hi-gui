#ifndef HI_ATTRIBUTE_H
#define HI_ATTRIBUTE_H

#include "hitypes.h"

namespace higui
{
	class Attribute 
	{
	public:
		Attribute(const std::string& key = "str");
		Attribute(const std::string& key, const std::string& new_value);

		// for std::shared_ptr
		template <typename T>
		std::enable_if_t<std::is_same<T, std::shared_ptr<typename std::decay_t<T>::element_type>>::value, 
			Attribute&>
			operator=(T&& new_value)
		{
			value = std::forward<T>(new_value);
			return *this;
		}

		// T is std::string 
		Attribute& operator=(const std::string& new_value)
		{
			value->fromString(new_value);
			return *this;
		}

		// for cloning
		template <typename T>
		std::enable_if_t<!std::is_pointer_v<T> && std::is_base_of<internal::AttributeValueBase, std::decay_t<T>>::value, Attribute&>
			operator=(const T& new_value)
		{
			value = std::make_shared<typename std::decay<T>::type>(new_value);
			return *this;
		}

		template <typename T>
		std::enable_if_t<
			std::is_same_v<T, int> || 
			std::is_same_v<T, float> || 
			std::is_same_v<T, double>,
		Attribute&> operator=(const T& new_value)
		{
			value->fromString(std::to_string(new_value));
			return *this;
		}

		void setKey(const std::string& key)
		{
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

		std::string key() const {
			return key_;
		}

		friend std::ostream& operator<<(std::ostream& os, const Attribute& obj)
		{
			os << "key: " << obj.key() << "\t";
			if (std::dynamic_pointer_cast<AttributeString>(obj.value)) {
				os << "value: \"" << obj.value->toString() << "\"";
			} else {
				os << "value: "   << obj.value->toString();
			}
			return os;
		}

		std::shared_ptr<internal::AttributeValueBase> value;

	private:
		std::string key_;
	};

	class AttributeContainer {
	public:
		void add(const std::string& key) {
			attributes_.push_back(Attribute(key));
		}

		void add(Attribute attribute) {
			attributes_.push_back(attribute);
		}

		void add(const std::string& key, const std::string& value) {
			attributes_.push_back(Attribute(key, value));
		}

		template <typename T>
		std::enable_if_t<!std::is_pointer_v<T> && std::is_base_of<internal::AttributeValueBase, std::decay_t<T>>::value, void>
			add(std::string key, T& new_value)
		{
			Attribute attr{ key };
			attr = std::make_shared<typename std::decay<T>::type>(new_value);
			attributes_.push_back(attr);
		}

		template <typename T>
		void add(const std::string& key, const T& value)
		{
			Attribute attr{ key };
			attr = value;
			attributes_.push_back(attr);
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

		template <typename Derived>
		Derived& get(const std::string& key) {
			std::size_t attr_index = find(key);
			if (attr_index != npos) {
				if (std::shared_ptr<Derived> derived_value = std::dynamic_pointer_cast<Derived>(attributes_[attr_index].value)) {
					return *derived_value;
				}
				else {
					throw std::runtime_error("Attribute value cannot be converted to the specified type.");
				}
			}
			std::shared_ptr<Derived> new_attribute = std::make_shared<Derived>();
			Attribute attr(key);
			attr.value = new_attribute;
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