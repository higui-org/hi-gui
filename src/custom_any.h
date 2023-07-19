#ifndef CUSTOM_ANY_H
#define CUSTOM_ANY_H

#include <map>
#include <string>
#include <sstream>
#include <any>
#include <typeinfo>
#include <stdexcept>
#include <glm/glm.hpp>

namespace higui
{
    template<typename T>
    T AnyCast(const std::any& value)
    {
        if (auto result = std::any_cast<T>(&value))
        {
            return *result;
        }
        else
        {
            throw std::bad_any_cast();
        }
    }

    struct RGBColor
    {
        size_t r;
        size_t g;
        size_t b;
    };

    template <typename T>
    T FromString(const std::string& input)
    {
        std::stringstream ss(input);
        std::any result;

        auto CreateAny = [](const auto& value)
        {
            return std::any(value);
        };

        // hex value
        if (input.front() == '#' && input.length() == 7)
        {
            unsigned int hexValue;
            std::stringstream hexStream(input.substr(1));
            hexStream >> std::hex >> hexValue;
            size_t r = (hexValue >> 16) & 0xFF;
            size_t g = (hexValue >> 8) & 0xFF;
            size_t b = hexValue & 0xFF;
            result = CreateAny(RGBColor{ r, g, b });
        }
        // rgb value
        else if (input.substr(0, 4) == "rgb(" && input.back() == ')')
        {
            RGBColor rgb;
            char delimiter;
            ss.ignore(4);
            ss >> rgb.r >> delimiter >> rgb.g >> delimiter >> rgb.b;
            if (delimiter == ',')
            {
                result = CreateAny(rgb);
            }
        }
        // percentage value
        else if (input.back() == '%')
        {
            float value;
            ss >> value;
            result = CreateAny(value / 100.0f);
        }
        // int value
        else if (ss >> std::noskipws)
        {
            int value;
            ss >> value;
            result = CreateAny(value);
        }
        else
        {
            throw std::invalid_argument("Invalid input format for FromString.");
        }

        try
        {
            return std::any_cast<T>(result);
        }
        catch (const std::bad_any_cast&)
        {
            throw std::invalid_argument("Failed to convert to the specified type.");
        }
    }

    std::string ToString(const RGBColor& value);

    std::string ToString(float value);

    std::string ToString(int value);

}

#endif // CUSTOM_ANY_H