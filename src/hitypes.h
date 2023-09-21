#ifndef HI_TYPES_H
#define HI_TYPES_H

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

namespace higui
{
	enum class DockPosition {
		None,
		Top,
		Left,
		Bottom,
		Right
	};

	// dont inherit from this. Use AttributeTagImpl.
	class AttributeTag {
	public:
		virtual ~AttributeTag() = default;

		virtual std::string toString() = 0;
		virtual void fromString(const std::string& tag) = 0;

		virtual void setValue(const std::any value) = 0;
		virtual void setValue(const std::string value) = 0;
	protected:
		static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeTag>()>>& registry()
		{
			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeTag>()>> registry;
			return registry;
		}

	private:
		friend class Attribute;
	};

	// for custom attribute tags
	template <typename Derived>
	class AttributeTagImpl : public AttributeTag {
	public:
		AttributeTagImpl() {}

		static void Register(const std::string& type)
		{
			RegisterType(type, []() -> std::shared_ptr<AttributeTag>
			{
				return std::make_shared<Derived>();
			});
		}

		virtual void setValueFrom(Derived another) = 0;

	protected:
		std::string getSubTagByIndex(const std::string& tag, size_t index) {
			size_t start_i = 0;
			size_t end_i = tag.find(' ');

			while (index > 0 && end_i != std::string::npos) {
				start_i = end_i + 1;
				end_i = tag.find(' ', start_i);
				index--;
			}

			if (start_i != std::string::npos) {
				if (end_i == std::string::npos) {
					if (index != 0)
					{
						return "";
					}
					return tag.substr(start_i);
				}
				else {
					return tag.substr(start_i, end_i - start_i);
				}
			}
			else {
				return "";
			}
		}

	private:
		void setValue(const std::any value) override
		{
			if (value.type() == typeid(Derived)) {
				std::shared_ptr<Derived> new_value = std::dynamic_pointer_cast<Derived>(value);
				setValueFrom(*(new_value.get()));
			}
		}

		void setValue(const std::string value) override
		{
			std::shared_ptr<Derived> derived_value = std::make_shared<Derived>();
			derived_value->fromString(value);
			setValueFrom(*(derived_value.get()));
		}

		static void RegisterType(const std::string& type, std::function<std::shared_ptr<AttributeTag>()> factory) {
			registry()[type] = factory;
		}
	};


	class IntAttributeTag : public AttributeTagImpl<IntAttributeTag> {
	public:
		IntAttributeTag() : int_value(0) {}

		std::string toString() override {
			return std::to_string(int_value);
		}

		void fromString(const std::string& value) override {
			int_value = std::stoi(value);
		}

		void setValueFrom(IntAttributeTag another) override {
			int_value = another.int_value;
		}

		int int_value;
	};

	class FloatAttributeTag : public AttributeTagImpl<FloatAttributeTag> {
	public:
		FloatAttributeTag() : float_value(0.0f) {}

		std::string toString() override {
			return std::to_string(float_value);
		}

		void fromString(const std::string& value) override {
			float_value = std::stof(value);
		}

		void setValueFrom(FloatAttributeTag another) override {
			float_value = another.float_value;
		}

		float float_value;
	};

	class StringAttributeTag : public AttributeTagImpl<StringAttributeTag> {
	public:
		StringAttributeTag() : str("") {}

		std::string toString() override {
			return str;
		}

		void fromString(const std::string& value) override {
			str = value;
		}

		void setValueFrom(StringAttributeTag another) override {
			str = another.str;
		}

		std::string str;
	};

	class Dock : public AttributeTagImpl<Dock> {
	public:

		Dock() : pos(DockPosition::None), ratio(0.5f) {}

		std::string toString() override;

		void fromString(const std::string& new_value) override;

		void setValueFrom(Dock another) override {
			pos = another.pos;
			ratio = another.ratio;
		}

		DockPosition pos;
		float ratio;
	};

	struct RGBA {
		size_t r, g, b, a;
		RGBA(size_t r, size_t g, size_t b, size_t a) : r(r), g(g), b(b), a(a) {}
	};

	namespace internal {
		RGBA ToRGBA(const std::string& value);

		float ToFloat(const std::string& value);

		int ToInt(const std::string& value);
	}

}

#endif // HI_TYPES_H
