#ifndef HI_TYPES_H
#define HI_TYPES_H

namespace higui
{
	class AttributeValue {
	public:
		virtual ~AttributeValue() = default;

		virtual void set(const std::string& value) = 0;
		
		virtual std::any get() = 0;

		virtual std::string toString() = 0;

		template <typename T>
		void set(const T& value) {
			throw std::runtime_error("Type not supported");
		}

	protected:
		static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValue>()>>& registry()
		{
			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValue>()>> registry;
			return registry;
		}

	private:
		friend class Attribute;
	};

	template <typename Derived>
	class AttributeValueImpl : public AttributeValue {
	public:
		static void RegisterAs(const std::string& type)
		{
			RegisterAs(type, []() -> std::shared_ptr<AttributeValue>
			{
				return std::make_shared<Derived>();
			});
		}

	private:
		static void RegisterAs(const std::string& type, std::function<std::shared_ptr<AttributeValue>()> factory) {
			registry()[type] = factory;
		}
	};

	class IntAttributeValue : public AttributeValueImpl<IntAttributeValue> {
	public:
		IntAttributeValue() : int_value(0) {}
		IntAttributeValue(int value) : int_value(value) {}

		void set(const std::string& value) override 
		{ int_value = std::stoi(value); }

		void set(const int& value)
		{ int_value = value; }

		std::string toString() override 
		{ return std::to_string(int_value);}

		std::any get() override 
		{ return int_value; }

	private:
		int int_value;
	};

	class FloatAttributeValue : public AttributeValueImpl<FloatAttributeValue> {
	public:
		FloatAttributeValue() : float_value(0.0f) {}
		FloatAttributeValue(float value) : float_value(value) {}

		void set(const std::string& value) override
		{ float_value = std::stof(value); }

		std::string toString() override
		{ return std::to_string(float_value); }

		std::any get() override 
		{ return float_value; }

	private:
		float float_value;
	};

	class StringAttributeValue : public AttributeValueImpl<StringAttributeValue> {
	public:
		StringAttributeValue() : str("") {}
		StringAttributeValue(std::string value) : str(value) {}

		void set(const std::string& value) override
		{ str = value; }

		std::string toString() override 
		{ return str; }

		std::any get() override 
		{ return str; }

	private:
		std::string str;
	};

	class Dock : public AttributeValueImpl<Dock>
	{
	public:
		enum class Position
		{
			None,
			Top,
			Left,
			Bottom,
			Right
		};

		Dock() : pos(Dock::Position::None) {}
		Dock(Dock::Position value) : pos(value) {}

		void set(const std::string& new_value) override
		{
			char dock_char = new_value.at(0);

			switch (dock_char)
			{
			case 't':
				pos = Position::Top;
				break;
			case 'l':
				pos = Position::Left;
				break;
			case 'b':
				pos = Position::Bottom;
				break;
			case 'r':
				pos = Position::Right;
				break;
			default:
				pos = Position::None;
				break;
			}
		}

		void set(const Dock::Position& position)
		{ pos = position; }

		std::string toString() override
		{
			switch (pos)
			{
			case Position::Top:
				return "top";
			case Position::Left:
				return "left";
			case Position::Bottom:
				return "bottom";
			case Position::Right:
				return "right";
			default:
				return "none";
			}
		}

		std::any get() override
		{ return pos; }

	private:
		Position pos;
	};

	struct RGBA {
		size_t r, g, b, a;
		RGBA(size_t r, size_t g, size_t b, size_t a) : r(r), g(g), b(b), a(a) {}
	};

	namespace internal
	{
		RGBA ToRGBA(const std::string& value) {
			int r = 0, g = 0, b = 0, a = 255;

			// rgb | rgba
			if (sscanf_s(value.c_str(), "rgb(%d,%d,%d)", &r, &g, &b) == 3) { return RGBA(r, g, b, 255); }
			else if (sscanf_s(value.c_str(), "rgba(%d,%d,%d,%d)", &r, &g, &b, &a) == 4) { return RGBA(r, g, b, a); }

			// hex
			else if (sscanf_s(value.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a) == 4) { return RGBA(r, g, b, a); }
			else if (sscanf_s(value.c_str(), "#%02x%02x%02x", &r, &g, &b) == 3) { return RGBA(r, g, b, 255); }

			throw std::runtime_error("Invalid RGBA format");
		}

		// percentage value as string to normalized float: "0%", "25%", "66.6666%", "100%"
		float ToFloat(const std::string& value) {
			std::smatch match;
			if (std::regex_match(value, match, std::regex(R"(\s*(\d+(\.\d+)?%)\s*)"))) 
			{
				float percentage = std::stof(match[1]);
				return percentage / 100.0f;
			}
			throw std::runtime_error("Invalid percentage format");
		}


		int ToInt(const std::string& value) {
			if (std::all_of(value.begin(), value.end(), ::isdigit)) {
				return std::stoi(value);
			}
			throw std::runtime_error("Invalid integer format");
		}
	}

}

#endif // HI_TYPES_H
