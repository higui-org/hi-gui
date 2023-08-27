#include "attribute.h"

namespace higui
{
    namespace string {
        RGB StringToRGB(const std::string& color)
        {
            // hex value: #000000
            if (color.front() == '#' && color.length() == 7)
            {
                unsigned int hexValue;
                std::stringstream hexStream(color.substr(1));
                hexStream >> std::hex >> hexValue;
                size_t r = (hexValue >> 16) & 0xFF;
                size_t g = (hexValue >> 8) & 0xFF;
                size_t b = hexValue & 0xFF;
                return { r, g, b };
            }
            // rgb value: rgb(0, 0, 0)
            else if (color.substr(0, 4) == "rgb(" && color.back() == ')')
            {
                std::stringstream ss(color);
                RGB rgb;
                char delimiter;
                ss.ignore(4);
                ss >> rgb.r >> delimiter >> rgb.g >> delimiter >> rgb.b;
                if (delimiter == ',')
                {
                    return rgb;
                }
            }
            throw std::runtime_error("StringToRGBA: Unsupported type.");
        }

        RGBA StringToRGBA(const std::string& color)
        {
            // hex value: #00000000
            if (color.front() == '#' && color.length() == 7)
            {
                unsigned int hexValue;
                std::stringstream hexStream(color.substr(1));
                hexStream >> std::hex >> hexValue;
                size_t r = (hexValue >> 24) & 0xFF;
                size_t g = (hexValue >> 16) & 0xFF;
                size_t b = (hexValue >> 8) & 0xFF;
                size_t a = hexValue & 0xFF;
                return { r, g, b, a };
            }
            // rgb value: rgba(0, 0, 0, 0)
            else if (color.substr(0, 5) == "rgba(" && color.back() == ')')
            {
                std::stringstream ss(color);
                RGBA rgba;
                char delimiter;
                ss.ignore(5);
                ss >> rgba.r >> delimiter >> rgba.g >> delimiter >> rgba.b >> delimiter >> rgba.a;
                if (delimiter == ',')
                {
                    return rgba;
                }
            }
            throw std::runtime_error("StringToRGBA: Unsupported type.");
        }

        float StringPercentageToFloat(const std::string& input)
        {
            std::stringstream ss(input);
            if (input.back() == '%')
            {
                float value;
                ss >> value;
                return value / 100.0f;
            }
            throw std::runtime_error("StringPercentageToFloat: Unsupported type.");
        }

        std::string ToString(const RGB& value)
        {
            std::stringstream ss;
            ss << "rgb(" << value.r << ", " << value.g << ", " << value.b << ")";
            return ss.str();
        }

        std::string ToString(const RGBA& value)
        {
            std::stringstream ss;
            ss << "rgba(" << value.r << ", " << value.g << ", " << value.b << ", " << value.a << ")";
            return ss.str();
        }

        std::string ToString(float value)
        {
            std::stringstream ss;
            ss << (value * 100) << "%";
            return ss.str();
        }

        std::string ToString(int value)
        {
            return std::to_string(value);
        }
    }
}