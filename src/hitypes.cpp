#include "hitypes.h"

namespace higui
{
	void Dock::fromString(const std::string& tag) {
		std::string dock_str = getSubTagByIndex(tag, 0);
		if (!dock_str.empty())
		{
			if (dock_str == "left")
			{
				pos = DockPosition::Left;
			}
			else if (dock_str == "top")
			{
				pos = DockPosition::Top;
			}
			else if (dock_str == "right")
			{
				pos = DockPosition::Right;
			}
			else if (dock_str == "bottom")
			{
				pos = DockPosition::Bottom;
			}
		}
		
		if (!getSubTagByIndex(tag, 1).empty())
		{
			std::cout << getSubTagByIndex(tag, 1) << std::endl;
			ratio = internal::ToFloat(getSubTagByIndex(tag, 1));
		}

	}

	std::string Dock::toString() {
		switch (pos) {
		case DockPosition::Top:
			return "top";
		case DockPosition::Left:
			return "left";
		case DockPosition::Bottom:
			return "bottom";
		case DockPosition::Right:
			return "right";
		default:
			return "none";
		}
	}

	namespace internal {
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

		float ToFloat(const std::string& value) {
			std::smatch match;
			if (std::regex_match(value, match, std::regex(R"(\s*(\d+(\.\d+)?%)\s*)"))) {
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