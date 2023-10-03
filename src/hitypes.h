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

		namespace attr
		{
			class ValueBase {
			public:
				virtual ~ValueBase() = default;

				virtual std::string toString() = 0;
				virtual void fromString(const std::string& value) = 0;

			protected:

				static std::unordered_map<std::string, std::function<std::shared_ptr<ValueBase>()>>& registry()
				{
					static std::unordered_map<std::string, std::function<std::shared_ptr<ValueBase>()>> registry;
					return registry;
				}

				friend class Attribute;
			};
		}
	};

	namespace attr
	{
		template <typename Derived>
		class Value : public internal::attr::ValueBase {
		public:
			Value() {}

			static void Register(const std::string& type)
			{
				RegisterType(type, []() -> std::shared_ptr<internal::attr::ValueBase>
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
			static void RegisterType(const std::string& type, std::function<std::shared_ptr<internal::attr::ValueBase>()> factory) {
				registry()[type] = factory;
			}
		};


		class Int : public Value<Int> {
		public:
			Int() : int_value(0) {}

			std::string toString() override {
				return std::to_string(int_value);
			}

			void fromString(const std::string& value) override {
				int_value = std::stoi(value);
			}

			int int_value;
		};

		class Float : public Value<Float> {
		public:
			Float() : float_value(0.0f) {}

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

		class String : public Value<String> {
		public:
			String() : str("") {}

			std::string toString() override {
				return str;
			}

			void fromString(const std::string& value) override {
				str = value;
			}

			std::string str;
		};

		class Alignment : public Value<Alignment> {
		public:

			Alignment() : pos(Align::None), ratio(0.5f) {}
			Alignment(Align alignment, float ratio = 0.5f) : pos(alignment), ratio(ratio) {}

			std::string toString() override;

			void fromString(const std::string& new_value) override;


			Align pos;
			float ratio;
		};
	}
}

#endif // HI_TYPES_H
