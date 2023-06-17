#ifndef CUSTOM_ANY_H
#define CUSTOM_ANY_H

#include <iostream>
#include <map>
#include <string>
#include <any>
#include <typeinfo>
#include <stdexcept>

namespace higui
{

    template<typename KeyType>
    class AnyMap {
    public:
        template<typename ValueType>
        void insert(const KeyType& key, const ValueType& value) {
            map_[key] = value;
        }

        template<typename ValueType>
        ValueType& at(const KeyType& key) {
            return std::any_cast<ValueType&>(map_.at(key));
        }

        template<typename ValueType>
        const ValueType& at(const KeyType& key) const {
            return std::any_cast<const ValueType&>(map_.at(key));
        }

        void erase(const KeyType& key) {
            map_.erase(key);
            
        }

        bool contains(const KeyType& key) const {
            return map_.find(key) != map_.end();
        }

        class iterator {
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
    T AnyCast(const std::any& value) {
        if (value.type() != typeid(T)) {
            throw std::bad_cast();
        }
        return std::any_cast<T>(value);
    }
}

#endif // CUSTOM_ANY_H