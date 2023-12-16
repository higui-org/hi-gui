#include "hitypes.h"

namespace hi
{
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