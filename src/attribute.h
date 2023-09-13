#ifndef HI_ATTRIBUTE_H
#define HI_ATTRIBUTE_H

#include "hitypes.h"

namespace higui
{
	class Attribute 
	{
	public:
		Attribute(const std::string& key);
		Attribute(const std::string& key, const std::string& value);

		template <typename T>
		std::enable_if_t<std::is_convertible_v<T, std::string> || std::is_same_v<T, const char*>, Attribute&> operator=(const T& new_value) {
			attr_value->fromString(std::string(new_value));
			return *this;
		}

		template <typename T>
		std::enable_if_t<!std::is_convertible_v<T, std::string> && !std::is_same_v<T, const char*>, Attribute&> operator=(const T& new_value) {
			attr_value->setValue(std::any(new_value));
			return *this;
		}



		std::string key() const {
			return key_;
		}

		std::string value_str() const {
			return attr_value->toString();
		}

		template <typename T>
		T value() const {
			return std::any_cast<T>(attr_value->getValue());
		}

		std::any value_any() const {
			return attr_value->getValue();
		}

		friend std::ostream& operator<<(std::ostream& os, const Attribute& obj)
		{
			os << "Attribute(key: " << obj.key() << ", value: " << obj.attr_value->toString() << ")";
			return os;
		}

	private:
		std::string key_;
		std::shared_ptr<AttributeValue> attr_value;
	};

	class AttributeContainer : public std::list<Attribute> {
	public:
		Attribute& operator[](const std::string& key) {
			iterator it = find(key);

			if (it != end())
				return *it;

			emplace_back(key, "");
			return back();
		}

		Attribute& get(const std::string& key) {
			auto it = find(key);

			if (it != end()) 
				return *it;

			throw std::runtime_error("AttributeContainer does not contain Attribute with key: " + key);
		}

		// contains attribute
		bool contains(const std::string& key) {
			return find(key) == end() ? false : true;
		}

		iterator find(const std::string& key) {
			for (auto it = begin(); it != end(); ++it) {
				if (it->key() == key) {
					return it;
				}
			}
			return end();
		}

		// erase from container by key
		void erase(const std::string& key) {
			erase(find(key));
		}

		void erase(iterator it) {
			std::list<Attribute>::erase(it);
		}

		// size
		size_t size() const {
			return std::list<Attribute>::size();
		}

		// clear all attributes
		void clear() {
			std::list<Attribute>::clear();
		}
	};
}

#endif // HI_ATTRIBUTE_H