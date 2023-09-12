#ifndef HI_ATTRIBUTE_H
#define HI_ATTRIBUTE_H

#include <iostream>
#include <map>
#include <typeindex>
#include <functional>
#include <any>
#include <string>
#include <memory>
#include <stdexcept>
#include <regex>
#include <list>
#include <sstream>

#include "hitypes.h"

namespace higui
{
	class Attribute {
	public:
		Attribute(const std::string& key, const std::string& value) : key_(key) 
		{
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
				new_value->set(real_value);
				attr_value = new_value;
			}
			else {
				throw std::runtime_error("Invalid Attribute type: " + type);
			}
		}

		template <typename T>
		Attribute& operator=(const T& new_value)
		{
			auto it = AttributeValue::registry().find(key_);

			if (it != AttributeValue::registry().end()) {
				std::shared_ptr<AttributeValue> value = it->second();

				if (dynamic_cast<AttributeValueImpl<T>*>(value.get())) {
					value->set(new_value);
					attr_value = value;
				}
				else {
					throw std::runtime_error("Type mismatch");
				}
			}
			else {
				throw std::runtime_error("Type not supported");
			}

			return *this;
		}

		std::string key() const {
			return key_;
		}

		std::string value_str() const {
			return attr_value->toString();
		}

		template <typename T>
		T value () const {
			return std::any_cast<T>(attr_value->get());
		}

		std::any value_any() const
		{
			return attr_value->get();
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