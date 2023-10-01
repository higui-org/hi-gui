#include "hitypes.h"

namespace higui
{
	void AttributeDock::fromString(const std::string& tag) {
		std::vector<std::string> splitted = SplitBySpace(tag);
		
		std::string dock_str = splitted[0];
		std::string ratio_str = splitted[1];

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
		if (!ratio_str.empty())
		{
			ratio = internal::ToNormalizedFloat(ratio_str);
		}

	}

	std::string AttributeDock::toString() {
		switch (pos) {
		case DockPosition::Top:
			return "top " + std::to_string(ratio);
		case DockPosition::Left:
			return "left " + std::to_string(ratio);
		case DockPosition::Bottom:
			return "bottom " + std::to_string(ratio);
		case DockPosition::Right:
			return "right " + std::to_string(ratio);
		default:
			return "none " + std::to_string(ratio);
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

		float ToNormalizedFloat(const std::string& value) {
			std::smatch match;
			if (std::regex_match(value, match, std::regex(R"(\s*(\d+(\.\d+)?%)\s*)"))) {
				float percentage = std::stof(match[1]);
				return percentage / 100.0f;
			}
			throw std::runtime_error("Invalid percentage format");
		}
	}
}