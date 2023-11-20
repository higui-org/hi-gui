#ifndef Hi_GUI_OBJECT_H
#define Hi_GUI_OBJECT_H

// glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// higui
#include "attribute.h"
#include "shader.h"

namespace higui
{
	class DOM;
	class Markup;
	namespace internal
	{ class GUIObjectBase; }

	using GUIObjectPtr = std::shared_ptr<internal::GUIObjectBase>;

	namespace internal
	{
		class GUIObjectBase : public std::enable_shared_from_this<GUIObjectBase>
		{
		public:
			GUIObjectBase() : parent(nullptr), model(1.0f) {}

			virtual void Render(unsigned int VAO);
			virtual void Update();

			template <class AttributeValue>
			std::enable_if_t<std::is_base_of_v<AttributeValueBase, AttributeValue>, AttributeValue&>
				attr(const std::string& key = "")
			{
				return container.value<AttributeValue>();
			}

			Attribute& attr(const std::string& key) 
			{ 
				return container[key]; 
			}



			GUIObjectPtr getParent() { return parent; }
			glm::mat4 getModel() { return model; }
			void ResetModel(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);

			// normalized
			glm::vec3 Position();
			glm::vec3 Scale();
			glm::quat Rotation();
			geometry3 Geometry();

			// screen
			glm::vec3 ScreenCoords(GLFWwindow* window);
			glm::vec3 ScreenDimensions(GLFWwindow* window);
			geometry3 ScreenGeometry(GLFWwindow* window);

			// events
			virtual bool OnCursorPos(double xpos, double ypos);
			virtual bool OnMouseClick(int button, double xpos, double ypos);

			// output
			friend std::ostream& operator<<(std::ostream& os, const GUIObjectPtr& obj);

		protected:
			void AddChild(GUIObjectPtr obj_ptr) { children.push_back(obj_ptr); }

			GUIObjectPtr MouseIn(glm::vec2 point, GLFWwindow* win);

			// attributes
			AttributeContainer container;

			std::vector<GUIObjectPtr> children{};
			GUIObjectPtr parent;
			glm::mat4 model;

			// basic rectangle
			static float vertices[12];
			static unsigned int indices[6];

			// friends~
			friend class DOM;
			friend class Markup;

			static std::unordered_map<std::string, std::function<GUIObjectPtr()>>& registry()
			{
				static std::unordered_map<std::string, std::function<GUIObjectPtr()>> registry;
				return registry;
			}
		};
	} // end of 'internal' namespace

	template<class Derived>
	class GUIObject : public internal::GUIObjectBase
	{
	public:
		// register markup tag
		static void Register(const std::string& type)
		{
			RegisterType(type, []() -> std::shared_ptr<Derived>
			{
				return std::make_shared<Derived>();
			});
		}

	private:
		static void RegisterType(const std::string& type, std::function<GUIObjectPtr()> factory) {
			registry()[type] = factory;
		}
	};

	namespace tag
	{
		class Object : public GUIObject<Object> {};
	}
}

#endif // Hi_GUI_OBJECT_H
