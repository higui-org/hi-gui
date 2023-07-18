#include "custom_any.h"

namespace higui
{
    std::string ToString(const RGBColor& value)
    {
        std::stringstream ss;
        ss << "rgb(" << value.r << ", " << value.g << ", " << value.b << ")";
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