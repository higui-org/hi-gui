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

	class AttributeValue {
	public:
		virtual ~AttributeValue() = default;

		virtual std::string toString() = 0;
		virtual void fromString(const std::string& value) = 0;

		virtual std::any getValue() = 0;
		virtual void setValue(const std::any& value) = 0;
	protected:
		static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValue>()>>& registry()
		{
			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValue>()>> registry;
			return registry;
		}

	private:
		friend class Attribute;
	};

	template <typename Derived, typename T>
	class AttributeValueImpl : public AttributeValue {
	public:
		AttributeValueImpl() : value_(T()) {}

		static void RegisterAs(const std::string& type)
		{
			RegisterAs(type, []() -> std::shared_ptr<AttributeValue>
			{
				return std::make_shared<Derived>();
			});
		}

	private:
		T parseFromString(const std::string& value) {

		}

		T value_;

		static void RegisterAs(const std::string& type, std::function<std::shared_ptr<AttributeValue>()> factory) {
			registry()[type] = factory;
		}
	};


	class IntAttributeValue : public AttributeValueImpl<IntAttributeValue, int> {
	public:
		IntAttributeValue() : int_value(0) {}

		std::string toString() override {
			return std::to_string(int_value);
		}

		void fromString(const std::string& value) override {
			int_value = std::stoi(value);
		}

		std::any getValue() override {
			return std::any(int_value);
		}

		void setValue(const std::any& value) override {
			int_value = std::any_cast<int>(value);
		}

	private:
		int int_value;
	};

	class FloatAttributeValue : public AttributeValueImpl<FloatAttributeValue, float> {
	public:
		FloatAttributeValue() : float_value(0.0f) {}

		std::string toString() override {
			return std::to_string(float_value);
		}

		void fromString(const std::string& value) override {
			float_value = std::stof(value);
		}

		std::any getValue() override {
			return std::any(float_value);
		}

		void setValue(const std::any& value) override {
			float_value = std::any_cast<float>(value);
		}

	private:
		float float_value;
	};

	class StringAttributeValue : public AttributeValueImpl<StringAttributeValue, std::string> {
	public:
		StringAttributeValue() : str("") {}
		StringAttributeValue(const std::string& value) : str(value) {}

		std::string toString() override {
			return str;
		}

		void fromString(const std::string& value) override {
			str = value;
		}

		std::any getValue() override {
			return std::any(str);
		}

		void setValue(const std::any& value) override {
			str = std::any_cast<std::string>(value);
		}

	private:
		std::string str;
	};

	class Dock : public AttributeValueImpl<Dock, DockPosition> {
	public:

		Dock() : pos(DockPosition::None) {}

		std::string toString() override;

		void fromString(const std::string& new_value) override;

		std::any getValue() override {
			return std::any(pos);
		}

		void setValue(const std::any& value) override {
			pos = std::any_cast<DockPosition>(value);
		}

	private:
		DockPosition pos;
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
