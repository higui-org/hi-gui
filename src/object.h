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
	using namespace internal;

	class DOM;
	class Markup;

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
				return attribute_container.value<AttributeValue>();
			}

			Attribute& attr(const std::string& key) { return attribute_container[key]; }

			std::shared_ptr<GUIObjectBase> getParent() { return parent; }
			glm::mat4 getModel() { return model; }
			void ResetModel(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);

			glm::vec3 Position();
			glm::vec3 Scale();
			glm::quat Rotation();
			geometry3 Geometry();

			glm::vec3 ScreenCoords(GLFWwindow* window);
			glm::vec3 ScreenDimensions(GLFWwindow* window);
			geometry3 ScreenGeometry(GLFWwindow* window);

			// events
			virtual bool OnCursorPos(double xpos, double ypos);
			virtual bool OnMouseClick(int button, double xpos, double ypos);

		protected:
			void AddChild(std::shared_ptr<GUIObjectBase> obj) { children.push_back(obj); }

			std::shared_ptr<GUIObjectBase> MouseIn(glm::vec2 point, GLFWwindow* win);

			// attributes
			AttributeContainer attribute_container;

			std::vector<std::shared_ptr<GUIObjectBase>> children{};
			std::shared_ptr<GUIObjectBase> parent;
			glm::mat4 model;

			// basic rectangle
			static float vertices[12];
			static unsigned int indices[6];

			// friends~
			friend class DOM;
			friend class Markup;

			static std::unordered_map<std::string, std::function<std::shared_ptr<GUIObjectBase>()>>& registry()
			{
				static std::unordered_map<std::string, std::function<std::shared_ptr<GUIObjectBase>()>> registry;
				return registry;
			}
		};
	} // end of 'internal' namespace

	template<class Derived>
	class GUIObject : public GUIObjectBase
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
		static void RegisterType(const std::string& type, std::function<std::shared_ptr<GUIObjectBase>()> factory) {
			registry()[type] = factory;
		}
	};

	namespace tag
	{
		class Object : public GUIObject<Object> {};
	}
}

#endif // Hi_GUI_OBJECT_H
