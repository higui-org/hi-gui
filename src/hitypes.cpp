#include "hitypes.h"

namespace higui
{
	void attribute::Alignment::from_str(const std::string& tag) {
		std::vector<std::string> splitted = SplitBySpace(tag);
		
		std::string dock_str, ratio_str;
		try
		{
			dock_str = splitted.at(0);
		}
		catch (std::out_of_range)
		{
			dock_str = "none";
		}

		try
		{
			ratio_str = splitted.at(1);
		}
		catch (std::out_of_range)
		{
			ratio_str = "25%";
		}

		if (!dock_str.empty())
		{
			if (dock_str == "left")
			{
				pos = Align::Left;
			}
			else if (dock_str == "top")
			{
				pos = Align::Top;
			}
			else if (dock_str == "right")
			{
				pos = Align::Right;
			}
			else if (dock_str == "bottom")
			{
				pos = Align::Bottom;
			}
		}
		if (!ratio_str.empty())
		{
			ratio = internal::ToNormalizedFloat(ratio_str);
		}

	}

	std::string attribute::Alignment::to_str() {
		switch (pos) {
		case Align::Top:
			return "top " + std::to_string(ratio);
		case Align::Left:
			return "left " + std::to_string(ratio);
		case Align::Bottom:
			return "bottom " + std::to_string(ratio);
		case Align::Right:
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

		bool isPointInsideRect(const glm::vec2& point, const glm::vec4& rect) {
			return point.x >= rect.x && point.x <= rect.x + rect.z &&
				   point.y >= rect.y && point.y <= rect.y + rect.w;
		}

		glm::vec4 RectsIntersect(const glm::vec4& a, const glm::vec4& b) noexcept {
			float x1 = std::max(a.x, b.x);
			float y1 = std::max(a.y, b.y);
			float x2 = std::min(a.x + a.z, b.x + b.z);
			float y2 = std::min(a.y + a.w, b.y + b.w);

			if (x1 < x2 && y1 < y2) {
				return glm::vec4(x1, y1, x2 - x1, y2 - y1); // intersect
			}
			else {
				return glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f); // rects doesnt intersect, return -1
			}
		}
	}
}