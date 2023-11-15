#ifndef Hi_TYPES_H
#define Hi_TYPES_H

#include <iostream>
#include <map>
#include <typeindex>
#include <functional>
#include <any>
#include <string>
#include <memory>
#include <stdexcept>
#include <regex>
#include <list>
#include <sstream>

#include <glm/glm.hpp>

namespace higui
{
	// further defined classes
	class Attribute;
	class AttributeContainer;

	// basic higui's align
	enum class Align {
		None,
		Top,
		Left,
		Bottom,
		Right
	};

	// basic higui's color
	struct RGBA {
		size_t r, g, b, a;
		RGBA(size_t r, size_t g, size_t b, size_t a) : r(r), g(g), b(b), a(a) {}
	};

	// geometry
	template <typename T, int Dimensions>
	struct geometry {
		glm::vec<Dimensions, T, glm::defaultp> pos;
		glm::vec<Dimensions, T, glm::defaultp> dim;

		geometry(const glm::vec<Dimensions, T, glm::defaultp>& p, const glm::vec<Dimensions, T, glm::defaultp>& d)
			: pos(p), dim(d) {}

		geometry(const geometry<T, Dimensions+1>& g)
			: pos(g.pos), dim(g.dim) {}
	};

	using geometry1 = geometry<float, 1>;
	using geometry2 = geometry<float, 2>;
	using geometry3 = geometry<float, 3>;
	using geometry4 = geometry<float, 4>;

	template <typename T, int D>
	std::ostream& operator<<(std::ostream& os, const geometry<T, D>& geo) {
		os << "geometry{ (";
		for (int i = 0; i < D; ++i) {
			os << geo.pos[i];
			if (i < D - 1) os << ", ";
		}
		os << "), (";
		for (int i = 0; i < D; ++i) {
			os << geo.dim[i];
			if (i < D - 1) os << ", ";
		}
		os << ") }";
		return os;
	}

	// internal
	namespace internal {

		// is Point Inside Geometry
		// point and geometry have the same number of dimensions
		template <typename T, int Dimensions>
		bool isPointInsideGeometry(const glm::vec<Dimensions, T, glm::defaultp>& point, const geometry<T, Dimensions>& geometry) {
			bool inside = true;
			for (int i = 0; i < Dimensions; i+=3) {
				inside &= point[i] >= geometry.pos[i] && point[i] <= geometry.pos[i] + geometry.dim[i];
			}
			return inside;
		}

		
		// intersect
		template <typename T, int Dimensions>
		geometry<T, Dimensions> getGeometriesIntersect(const geometry<T, Dimensions>& a, const geometry<T, Dimensions>& b)
		{
			static_assert(Dimensions >= 2, "getGeometriesIntersect can only be used with Dimensions >= 2");

			glm::vec<Dimensions, T, glm::defaultp> pos;
			glm::vec<Dimensions, T, glm::defaultp> dim;

			bool intersects = true;
			for (int i = 0; i < Dimensions; i+=3) {
				pos[i] = std::max(a.pos[i], b.pos[i]);
				T min_edge = std::min(a.pos[i] + a.dim[i], b.pos[i] + b.dim[i]);
				if (pos[i] < min_edge) {
					dim[i] = min_edge - pos[i];
				}
				else {
					intersects = false;
					break;
				}
			}

			if (intersects) {
				return { pos, dim };
			}
			else {
				return { glm::vec<Dimensions, T, glm::defaultp>(-1), glm::vec<Dimensions, T, glm::defaultp>(-1) };
			}
		}

		// distance
		// Basic version for the case when point and geometry have the same number of dimensions
		// point:    screen coordinates
		// geometry: screen geometry
		template <typename T, int Dimensions>
		T DistanceToGeometryCenter(const glm::vec<Dimensions, T, glm::defaultp>& point, const geometry<T, Dimensions>& geometry) {
			glm::vec<Dimensions, T, glm::defaultp> center = geometry.pos + (geometry.dim * T(0.5));
			return glm::distance(point, center);
		}


		// some converters
		RGBA ToRGBA(const std::string& value);
		float ToNormalizedFloat(const std::string& value);


		// attribute value base class
		class AttributeValueBase {
		public:
			virtual ~AttributeValueBase() = default;

			virtual std::string to_str() = 0;
			virtual void from_str(const std::string& value) = 0;
			virtual std::shared_ptr<AttributeValueBase> instance() const = 0;

		protected:

			static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>>& registry()
			{
				static std::unordered_map<std::string, std::function<std::shared_ptr<AttributeValueBase>()>> registry;
				return registry;
			}

			// friends~
			friend class Attribute;
			friend class AttributeContainer;
		};
	};
	// end of internal


	// basic attributes' values
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
}

#endif // HI_TYPES_H
