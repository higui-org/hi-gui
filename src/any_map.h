#ifndef OBJECTPROPERTY_H
#define OBJECTPROPERTY_H

#include <iostream>
#include <map>
#include <string>
#include <any>

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

private:
    std::map<KeyType, std::any> map_;
};


#endif // OBJECTPROPERTY_H