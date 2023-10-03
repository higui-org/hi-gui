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
	class Attribute;

	enum class DockPosition {
		None,
		Top,
		Left,
		Bottom,
		Right
	};

	struct RGBA {
		size_t r, g, b, a;
		RGBA(size_t r, size_t g, size_t b, size_t a) : r(r), g(g), b(b), a(a) {}
	};

	namespace internal {
		RGBA ToRGBA(const std::string& value);
		float ToNormalizedFloat(const std::string& value);

		class AttributeValueBase {
		public:
			virtual ~AttributeValueBase() = default;

			virtual std::string toString() = 0;
			virtual void fromString(const std::string& value) = 0;

		protected:

			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>>& registry()
			{
				static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>> registry;
				return registry;
			}

			friend class Attribute;
		};
	};

	template <typename Derived>
	class AttributeValue : public internal::AttributeValueBase {
	public:
		AttributeValue() {}

		static void Register(const std::string& type)
		{
			RegisterType(type, []() -> std::shared_ptr<internal::AttributeValueBase>
			{
				return std::make_shared<Derived>();
			});
		}

		friend std::ostream& operator<<(std::ostream& os, Derived obj)
		{
			os << obj.toString();
			return os;
		}

	protected:
		std::vector<std::string> SplitBySpace(const std::string& value) {
			std::vector<std::string> result;
			std::istringstream tokenizer(value);
			std::string word;

			while (tokenizer >> word) {
				result.push_back(word);
			}

			return result;
		}

	private:
		static void RegisterType(const std::string& type, std::function<std::shared_ptr<internal::AttributeValueBase>()> factory) {
			registry()[type] = factory;
		}
	};


	class AttributeInt : public AttributeValue<AttributeInt> {
	public:
		AttributeInt() : int_value(0) {}

		std::string toString() override {
			return std::to_string(int_value);
		}

		void fromString(const std::string& value) override {
			int_value = std::stoi(value);
		}

		int int_value;
	};

	class AttributeFloat : public AttributeValue<AttributeFloat> {
	public:
		AttributeFloat() : float_value(0.0f) {}

		std::string toString() override {
			return std::to_string(float_value);
		}

		void fromString(const std::string& value) override {
			try
			{
				float_value = std::stof(value);
			}
			catch (std::invalid_argument)
			{
				float_value = internal::ToNormalizedFloat(value);
			}
		}

		float float_value;
	};

	class AttributeString : public AttributeValue<AttributeString> {
	public:
		AttributeString() : str("") {}

		std::string toString() override {
			return str;
		}

		void fromString(const std::string& value) override {
			str = value;
		}

		std::string str;
	};

	class AttributeDock : public AttributeValue<AttributeDock> {
	public:

		AttributeDock() : pos(DockPosition::None), ratio(0.5f) {}
		AttributeDock(DockPosition pos, float ratio = 0.5f) : pos(pos), ratio(ratio) {}

		std::string toString() override;

		void fromString(const std::string& new_value) override;


		DockPosition pos;
		float ratio;
	};
}

#endif // HI_TYPES_H
