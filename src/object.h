#ifndef HI_GUI_OBJECT_H
#define HI_GUI_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "attribute.h"
#include "shader.h"

namespace higui
{
	class DOM;
	class Markup;

	namespace internal
	{
		class GUIObjectBase
		{
		public:
			GUIObjectBase() : parent(nullptr), model(1.0f) {}

			virtual void Render(unsigned int VAO) {}
			virtual void Update() {}

			template <class AttributeValue>
			std::enable_if_t<std::is_base_of_v<internal::attr::ValueBase, AttributeValue>, AttributeValue&>
				attr(const std::string& key)
			{
				return attribute[key].value<AttributeValue>();
			}

			Attribute& attr(const std::string& key) { return attribute[key]; }

			std::shared_ptr<GUIObjectBase> getParent() { return parent; }
			glm::mat4 getModel() { return model; }

			// returns a vector of pixels
			glm::vec2 Size(int framebuffer_width, int framebuffer_height);
			glm::vec2 Position(int framebuffer_width, int framebuffer_height);
			glm::vec4 Geometry(int framebuffer_width, int framebuffer_height);
			glm::vec2 Size(GLFWwindow* window);
			glm::vec2 Position(GLFWwindow* window);
			glm::vec4 Geometry(GLFWwindow* window);

			// events
			virtual bool OnCursorPos(double xpos, double ypos);
			virtual bool OnMouseClick(int button, double xpos, double ypos);

		protected:
			// Markup class adds children
			void AddChild(std::shared_ptr<GUIObjectBase> obj) { children.push_back(obj); }

			// attributes
			AttributeContainer attribute;

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
	}

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
		static void RegisterType(const std::string& type, std::function<std::shared_ptr<GUIObjectBase>()> factory) {
			registry()[type] = factory;
		}
	};
}

#endif // HI_GUI_OBJECT_H
