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

#include <glm/glm.hpp>

namespace higui
{
	class Attribute;
	class AttributeContainer;

	enum class Align {
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
		bool isPointInsideRect(const glm::vec2& point, const glm::vec4& rect);

		class AttributeValueBase {
		public:
			virtual ~AttributeValueBase() = default;

			virtual std::string to_str() = 0;
			virtual void from_str(const std::string& value) = 0;
			virtual std::shared_ptr<AttributeValueBase> instance() const = 0;

		protected:

			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>>& registry()
			{
				static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>> registry;
				return registry;
			}

			// friends~
			friend class Attribute;
			friend class AttributeContainer;
		};
	};

	namespace attribute
	{
		template <class Derived>
		class AttributeValue : public internal::AttributeValueBase {
		public:
			AttributeValue() {}
			virtual ~AttributeValue() = default;

			static void Register(const std::string& type)
			{
				RegisterType(type, []() -> std::shared_ptr<internal::AttributeValueBase>
				{
					return std::make_shared<Derived>();
				});
			}

			std::shared_ptr<internal::AttributeValueBase> instance() const override
			{
				return std::make_shared<Derived>();
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


		class Int : public AttributeValue<Int> {
		public:
			Int(int value = 0) : int_value(value) {}

			std::string to_str() override {
				return std::to_string(int_value);
			}

			void from_str(const std::string& value) override {
				int_value = std::stoi(value);
			}

			int int_value;
		};

		class Float : public AttributeValue<Float> {
		public:
			Float(float value = 0.0f) : float_value(value) {}

			std::string to_str() override {
				return std::to_string(float_value);
			}

			void from_str(const std::string& value) override {
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

		class String : public AttributeValue<String> {
		public:
			String(std::string str = "") : str(str) {}

			std::string to_str() override {
				return str;
			}

			void from_str(const std::string& value) override {
				str = value;
			}

			std::string str;
		};

		class Alignment : public AttributeValue<Alignment> {
		public:

			Alignment(Align alignment = Align::None, float ratio = 0.5f) : pos(alignment), ratio(ratio) {}

			std::string to_str() override;

			void from_str(const std::string& new_value) override;


			Align pos;
			float ratio;
		};
	}
}

#endif // HI_TYPES_H
