#ifndef CUSTOM_ANY_H
#define CUSTOM_ANY_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <any>
#include <typeinfo>
#include <stdexcept>
#include <glm/glm.hpp>

namespace higui
{

    template<typename KeyType>
    class AnyMap 
    {
    public:
        template<typename ValueType>
        void insert(const KeyType& key, const ValueType& value) 
        {
            map_[key] = value;
        }

        std::any& at(const KeyType& key) 
        {
            return map_.at(key);
        }

        template<typename ValueType>
        ValueType& at(const KeyType& key) 
        {
            return std::any_cast<ValueType&>(map_.at(key));
        }

        template<typename ValueType>
        const ValueType& at(const KeyType& key) const 
        {
            return std::any_cast<const ValueType&>(map_.at(key));
        }

        void erase(const KeyType& key) 
        {
            map_.erase(key);
            
        }

        bool contains(const KeyType& key) const 
        {
            return map_.find(key) != map_.end();
        }

        class iterator 
        {
        public:
            using difference_type = typename std::map<KeyType, std::any>::iterator::difference_type;
            using value_type = typename std::map<KeyType, std::any>::iterator::value_type;
            using pointer = typename std::map<KeyType, std::any>::iterator::pointer;
            using reference = typename std::map<KeyType, std::any>::iterator::reference;
            using iterator_category = typename std::map<KeyType, std::any>::iterator::iterator_category;

            iterator() = default;
            iterator(const typename std::map<KeyType, std::any>::iterator& it) : it_(it) {}

            reference operator*() const { return *it_; }
            pointer operator->() const { return it_.operator->(); }
            iterator& operator++() { ++it_; return *this; }
            iterator operator++(int) { iterator temp(*this); ++it_; return temp; }
            iterator& operator--() { --it_; return *this; }
            iterator operator--(int) { iterator temp(*this); --it_; return temp; }
            bool operator==(const iterator& other) const { return it_ == other.it_; }
            bool operator!=(const iterator& other) const { return it_ != other.it_; }

        private:
            typename std::map<KeyType, std::any>::iterator it_;
        };

        iterator begin() { return iterator(map_.begin()); }
        iterator end() { return iterator(map_.end()); }

    private:
        std::map<KeyType, std::any> map_;
    };



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

}

#endif // CUSTOM_ANY_H