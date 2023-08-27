#ifndef CUSTOM_ANY_H
#define CUSTOM_ANY_H

#include <map>
#include <string>
#include <sstream>
#include <any>
#include <unordered_map>
#include <functional>
#include <stdexcept>

namespace higui
{
    struct RGB
    {
        size_t r;
        size_t g;
        size_t b;
    };

    struct RGBA
    {
        size_t r;
        size_t g;
        size_t b;
        size_t a;
    };

    namespace string 
    {
        RGB StringToRGB(const std::string& color);
        RGBA StringToRGBA(const std::string& color);
        float StringPercentageToFloat(const std::string& input);

        std::string ToString(const RGB& value);
        std::string ToString(const RGBA& value);
        std::string ToString(float value);
        std::string ToString(int value);

        namespace internal
        {
            using ConversionFunction = std::function<std::any(const std::string&)>;
            using ConversionMap = std::unordered_map<std::string, ConversionFunction>;

            static ConversionMap conversion_map;
        }

        template<typename T>
        void RegisterConversionFunction(const std::string& type_name, std::function<T(const std::string&)> conversion_function)
        {
			internal::conversion_map.insert(std::pair<std::string, internal::ConversionFunction>(type_name, conversion_function));
        }
        
        template <typename T>
        T To(const std::string& input)
        {
            auto it = internal::conversion_map.find(typeid(T).name());
            if (it != internal::conversion_map.end())
            {
                const internal::ConversionFunction& conversion_function = it->second;
                return std::any_cast<T>(conversion_function(input));
            }
            else
            {
                throw std::invalid_argument("Unsupported type.");
            }
        }
    }
}

#endif // CUSTOM_ANY_H