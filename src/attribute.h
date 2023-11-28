#ifndef Hi_ATTRIBUTE_H
#define Hi_ATTRIBUTE_H

#include "hitypes.h"

namespace higui
{
	// basic attribute values
	namespace attribute
	{
		template <class Derived>
		class AttributeValue : public internal::AttributeValueBase {
		public:
			AttributeValue() {}
			virtual ~AttributeValue() = default;

			static void Register(const std::string& type)
			{
				RegisterType(type, []() -> std::shared_ptr<internal::AttributeValueBase>
				{
					return std::make_shared<Derived>();
				});
			}

			std::shared_ptr<internal::AttributeValueBase> instance() const override
			{
				return std::make_shared<Derived>();
			}

			friend std::ostream& operator<<(std::ostream& os, Derived obj)
			{
				return os << obj.to_str();
			}

		protected:
			std::vector<std::string> SplitBySpace(const std::string& value) {
				std::vector<std::string> result;
				std::istringstream tokenizer(value);
				std::string word;

				while (tokenizer >> word) {
					result.push_back(word);
				}

				return result;
			}

		private:
			static void RegisterType(const std::string& type, std::function<std::shared_ptr<internal::AttributeValueBase>()> factory) {
				registry()[type] = factory;
			}
		};


		class Int : public AttributeValue<Int> {
		public:
			Int(int value = 0) : int_value(value) {}

			std::string to_str() override {
				return std::to_string(int_value);
			}

			void from_str(const std::string& value) override {
				int_value = std::stoi(value);
			}

			int int_value;
		};


		class Float : public AttributeValue<Float> {
		public:
			Float(float value = 0.0f) : float_value(value) {}

			std::string to_str() override {
				return std::to_string(float_value);
			}
			void from_str(const std::string& value) override {
				try
				{
					float_value = std::stof(value);
				}
				catch (std::invalid_argument)
				{
					float_value = internal::ToNormalizedFloat(value);
				}
			}

			float float_value;
		};


		class String : public AttributeValue<String> {
		public:
			String(std::string str = "") : str(str) {}

			std::string to_str() override {
				return str;
			}
			void from_str(const std::string& value) override {
				str = value;
			}

			std::string str;
		};


		class Alignment : public AttributeValue<Alignment> {
		public:

			Alignment(Align alignment = Align::None, float ratio = 0.5f) : pos(alignment), ratio(ratio) {}

			std::string to_str() override;
			void from_str(const std::string& new_value) override;


			Align pos;
			float ratio;
		};


		class Bool : public AttributeValue<Bool>
		{
		public:
			Bool(bool value_ = false) : value(value_) {}

			std::string to_str() override;
			void from_str(const std::string& new_value) override;

			bool value;
		};
	}





	class Attribute 
	{
		using ValueBase = internal::AttributeValueBase;
		using ValuePtr = std::shared_ptr<internal::AttributeValueBase>;
	public:
		Attribute(const std::string& key = "str");
		Attribute(const std::string& key, const std::string& new_value);

		// for std::shared_ptr
		template <class T>
		std::enable_if_t<std::is_same<T, std::shared_ptr<class std::decay_t<T>::element_type>>::value,
			Attribute&>
			operator=(T&& new_value)
		{
			value_ = std::forward<T>(new_value);
			return *this;
		}

		// T is std::string 
		Attribute& operator=(const std::string& type_value);

		// for cloning
		template <class T>
		std::enable_if_t<!std::is_pointer_v<T> && std::is_base_of<internal::AttributeValueBase, std::decay_t<T>>::value, Attribute&>
			operator=(const T& new_value)
		{
			value_ = std::make_shared<class std::decay<T>::type>(new_value);
			return *this;
		}

		template <class T>
		std::enable_if_t<
			std::is_same_v<T, int> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, double>,
			Attribute&>
			operator=(T new_value)
		{
			if (typeid(T) == typeid(int))
			{
				value_ = std::make_shared<attribute::Int>(static_cast<int>(new_value));
			}
			else if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
			{
				value_ = std::make_shared<attribute::Float>(static_cast<float>(new_value));
			}
			else
			{
				value_ = std::make_shared<attribute::String>();
				value_->from_str(std::to_string(new_value));
			}
			return *this;
		}

		//void setKey(const std::string& key);

		std::string key() const { return key_; }

		template <class ValueClass>
		ValueClass& operator()()
		{ 
			ValueClass* v = dynamic_cast<ValueClass*>(value_.get());
			return *v; 
		}

		friend std::ostream& operator<<(std::ostream& os, const Attribute& obj);

		

	private:
		std::shared_ptr<internal::AttributeValueBase> value_;
		std::string key_;

		friend class AttributeContainer;
	};

	class AttributeContainer {
	public:
		AttributeContainer(const std::vector<Attribute>& attributes = {}) : elements(attributes) {}

		void add(const std::string& key) {
			elements.push_back(Attribute(key));
		}

		void add(Attribute attribute) {
			elements.push_back(attribute);
		}

		template <typename T>
		std::enable_if_t<!std::is_same_v<T, std::string>, 
		void> add(const std::string& key, const T& value)
		{
			Attribute attr{ key };
			attr = value;
			elements.push_back(attr);
		}

		void add(const std::string& key, const std::string& value) {
			elements.push_back(Attribute(key, value));
		}

		void remove(const std::string& key) {
			elements.erase(std::remove_if(elements.begin(), elements.end(),
				[key](const Attribute& attr) { return attr.key() == key; }), elements.end());
		}

		bool has(const std::string& key) const {
			for (const auto& attr : elements) {
				if (attr.key() == key) {
					return true;
				}
			}
			return false;
		}
		
		Attribute& operator[](const std::string& key) {
			std::size_t attr_index = find(key);
			if (attr_index != npos)
			{
				return elements[attr_index];
			}
			add(key);
			return elements.back();
		}

		template <class Derived>
		Derived& value(const std::string& key = "") {
			std::string effective_key = key;
			if (effective_key.empty()) {
				for (const auto& entry : internal::AttributeValueBase::registry()) {
					if (std::dynamic_pointer_cast<Derived>(entry.second())) {
						effective_key = entry.first;
						break;
					}
				}
				if (effective_key.empty()) {
					throw std::runtime_error("Could not determine key for the given type in the registry.");
				}
			}

			std::size_t attr_index = find(effective_key);
			if (attr_index != npos) {
				if (std::shared_ptr<Derived> derived_value = std::dynamic_pointer_cast<Derived>(elements[attr_index].value_)) {
					return *derived_value;
				}
				else {
					throw std::runtime_error("Attribute value cannot be converted to the specified type.");
				}
			}
			std::shared_ptr<Derived> new_attribute = std::make_shared<Derived>();
			Attribute attr(effective_key);
			attr.value_ = new_attribute;
			elements.push_back(attr);
			return *new_attribute;
		}

		Attribute& get(const std::string& key) {
			std::size_t attr_index = find(key);
			if (attr_index != npos)
			{
				return elements[attr_index];
			}
			throw std::runtime_error("Attribute key \"" + key + "\" not found");
		}

		std::size_t find(const std::string& key) const {
			for (std::size_t i = 0; i < elements.size(); ++i) {
				if (elements[i].key() == key) {
					return i;
				}
			}
			return npos;
		}

		class Iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = const Attribute;
			using reference = const Attribute&;
			using pointer = const Attribute*;
			using difference_type = std::ptrdiff_t;

			Iterator(std::vector<Attribute>::const_iterator it) : it_(it) {}

			reference operator*() const {
				return *it_;
			}

			pointer operator->() const {
				return &(*it_);
			}

			Iterator& operator++() {
				++it_;
				return *this;
			}

			Iterator operator++(int) {
				Iterator temp = *this;
				++it_;
				return temp;
			}

			bool operator==(const Iterator& other) const {
				return it_ == other.it_;
			}

			bool operator!=(const Iterator& other) const {
				return it_ != other.it_;
			}

		private:
			std::vector<Attribute>::const_iterator it_;
		};

		Iterator begin() const {
			return Iterator(elements.begin());
		}

		Iterator end() const {
			return Iterator(elements.end());
		}

	private:
		std::vector<Attribute> elements;
		static const std::size_t npos = -1;
	};
}

#endif // Hi_ATTRIBUTE_H