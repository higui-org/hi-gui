#ifndef HI_ATTRIBUTE_H
#define HI_ATTRIBUTE_H

#include "hitypes.h"

namespace higui
{
	class Attribute 
	{
	public:
		Attribute(const std::string& key = "str") {
			key_ = key;
			auto it = AttributeTag::registry().find(key_);
			if (it != AttributeTag::registry().end())
			{
				std::shared_ptr<AttributeTag> new_value = it->second();
				attr_value = new_value;
			}
			else {
				throw std::runtime_error("Invalid Attribute key: " + key_);
			}
		}

		Attribute(const std::string& key, const std::string& tag);

		template <typename T>
		Attribute(const std::string& key, const T& value)
		{
			key_ = key;
			attr_value->setValue(std::any(value));
		}

		template <typename T>
		std::enable_if_t<std::is_convertible_v<T, std::string> || std::is_same_v<T, const char*>, Attribute&> operator=(const T& new_value) {
			attr_value->fromString(std::string(new_value));
			return *this;
		}

		template <typename T>
		std::enable_if_t<!std::is_convertible_v<T, std::string> && !std::is_same_v<T, const char*>, Attribute&> operator=(const T& new_value) {
			attr_value->setValue(new_value);
			return *this;
		}

		void setKey(const std::string& key)
		{
			key_ = key;
			auto it = AttributeTag::registry().find(key_);
			if (it != AttributeTag::registry().end())
			{
				std::shared_ptr<AttributeTag> new_value = it->second();
				attr_value = new_value;
			}
			else {
				throw std::runtime_error("Invalid Attribute key: " + key_);
			}
		}

		std::string key() const {
			return key_;
		}

		std::string value_str() const {
			return attr_value->toString();
		}

		friend std::ostream& operator<<(std::ostream& os, const Attribute& obj)
		{
			os << "Attribute(key: " << obj.key();

			if (std::dynamic_pointer_cast<StringAttributeTag>(obj.attr_value)) {
				os << ", value: \"" <<  obj.attr_value->toString() << "\"";
			}
			else {
				os << ", value: " << obj.attr_value->toString();
			}
			os << ")";
			return os;
		}

	private:
		std::string key_;
		std::shared_ptr<AttributeTag> attr_value;
	};

	class AttributeContainer {
	public:
		void add(const std::string& key) {
			attributes_.push_back(Attribute(key));
		}

		void add(const std::string& key, const std::string& value) {
			attributes_.push_back(Attribute(key, value));
		}

		template <typename T>
		void add(const std::string& key, const T& value) {
			Attribute attr(key);
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