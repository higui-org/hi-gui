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

	// dont inherit from this. use GUIObjectImpl instead.
	class GUIObject
	{
	public:
		GUIObject();
		
		void Render(unsigned int VAO);
		virtual void Update();

		std::shared_ptr<GUIObject> getParent();
		glm::mat4 getModel();

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

		// attributes
		AttributeContainer attr;

	protected:
		// Markup class adds children
		void AddChild(std::shared_ptr<GUIObject> obj);

		std::vector<std::shared_ptr<GUIObject>> children;
		std::shared_ptr<GUIObject> parent;
		glm::mat4 model;

		// basic rectangle
		static float vertices[12];
		static unsigned int indices[6];

		// friends~
		friend class Markup;
		friend class DOM;

		static std::unordered_map<std::string, std::function<std::shared_ptr<GUIObject>()>>& registry()
		{
			static std::unordered_map<std::string, std::function<std::shared_ptr<GUIObject>()>> registry;
			return registry;
		}
	};

	// inherit from this GUIObjectImpl
	template<typename Derived>
	class GUIObjectImpl : public GUIObject
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
		static void RegisterType(const std::string& type, std::function<std::shared_ptr<GUIObject>()> factory) {
			registry()[type] = factory;
		}
	};
}

#endif // HI_GUI_OBJECT_H
