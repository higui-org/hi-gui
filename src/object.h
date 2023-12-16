#ifndef Hi_GUI_OBJECT_H
#define Hi_GUI_OBJECT_H

// glm
#include <glm/gtc/quaternion.hpp>

// higui
#include "attribute.h"
#include "component.h"
#include "shader.h"

namespace hi
{
	class GUIObject;

	using Widget = std::shared_ptr<GUIObject>;

	// class Transform
	class Transform
	{
	public:
		Transform() = default;

		void Translate(const glm::vec3& pos);
		void Scale(const glm::vec3& scale);
		void Rotate(const glm::quat& rotation);
		void Model(const glm::mat4& matrix);

		// normalized
		glm::vec3 getPos();
		glm::vec3 getScale();
		glm::quat getRotation();
		geometry<float, 3> getGeometry();

		// screen
		glm::vec3 getScreenPos(GLFWwindow* window);
		glm::vec3 getScreenScale(GLFWwindow* window); // get screen scale (screen dimensions)
		geometry<float, 3> getScreenGeometry(GLFWwindow* window);

		glm::mat4 getModel();

	private:
		glm::mat4 model;
	};







	//
	// internal
	//
	namespace internal
	{
		class GUIObjectBase : public std::enable_shared_from_this<GUIObjectBase>
		{
		public:
			GUIObjectBase(Widget parent_) : parent(parent_), transform() {}

			virtual void Render(unsigned int VAO) = 0;
			virtual void Update() = 0;

			void ResetModel(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);

			Widget getParent() { return parent; }

			// events
			virtual bool OnCursorPos(double xpos, double ypos);
			virtual bool OnMouseClick(int button, double xpos, double ypos);


			static std::unordered_map<std::string, std::function<Widget()>>& registry()
			{
				static std::unordered_map<std::string, std::function<Widget()>> registry;
				return registry;
			}

			Behavior behavior;
			Transform transform;

			virtual ~GUIObjectBase() = default;

		protected:
			void AddChild(Widget w) { children.push_back(w); }

			Widget MouseIn(glm::vec2 point, GLFWwindow* win);

			std::vector<Widget> children;
			Widget parent;

		private:
			// internal
			// friends~
			friend class DOM;
			friend class Markup;

			// basic rectangle
			static float vertices[12];
			static unsigned int indices[6];
		};
	} // end of 'internal' namespace

	class GUIObject : public internal::GUIObjectBase
	{

	};
}

#endif // Hi_GUI_OBJECT_H
