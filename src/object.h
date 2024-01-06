#ifndef Hi_GUI_OBJECT_H
#define Hi_GUI_OBJECT_H

// glm
#include <glm/gtc/quaternion.hpp>

// higui
#include "attribute.h"
#include "component.h"
#include "shader.h"

namespace hi {
	// class Transform
	template<typename T, int Dimensions>
	class Transform
	{
	public:
		Transform() = default;

		void Translate(const glm::vec<Dimensions, T, glm::highp>& pos);
		void Scale(const glm::vec<Dimensions, T, glm::highp>& scale);
		void Rotate(const glm::quat& rotation);
		void Model(const glm::mat4& matrix);

		// normalized
		glm::vec3 getPos();
		glm::vec3 getScale();
		glm::quat getRotation();
		geometry<float, Dimensions> getGeometry();

		// screen
		glm::vec3 getScreenPos(GLFWwindow* window);
		glm::vec3 getScreenScale(GLFWwindow* window); // get screen scale (screen dimensions)
		geometry<float, Dimensions> getScreenGeometry(GLFWwindow* window);

		glm::mat4 getModel();

	private:
		glm::mat4 model;
	};

	using Widget = std::shared_ptr<class GUIObject>;

	namespace internal 
	{
		template <typename T, int Dimensions>
		class GUIObjectBase : 
		public std::enable_shared_from_this<GUIObjectBase<T, Dimensions>> 
		{
		public:
		GUIObjectBase(Widget parent_) : parent(parent_), transform() {}

		virtual void Render(unsigned int VAO) = 0;
		virtual void Update() = 0;

		Widget getParent() { return parent; }

		static std::unordered_map<std::string, std::function<Widget()>>& registry() {
			static std::unordered_map<std::string, std::function<Widget()>> registry;
			return registry;
		}

		Transform<T, Dimensions> transform;

		virtual ~GUIObjectBase() = default;

		private:
			// friends~
			friend class DOM;
			friend class Markup;

			// basic rectangle
			static float vertices[12];
			static unsigned int indices[6];
		}; // class GUIObjectBase
	}; // namespace 'internal'

	class GUIObject : public internal::GUIObjectBase<float, 2>
	{
	public:
		GUIObject();
		~GUIObject();

	private:
		
	};

	
} // end of 'hi' namespace

#endif // Hi_GUI_OBJECT_H
