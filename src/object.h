#ifndef HI_GUI_OBJECT_H
#define HI_GUI_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "attribute.h"
#include "shader.h"

namespace higui
{
	class DOM;

	enum class ElementDock
	{
		none,
		top,
		bottom,
		left,
		right
	};

	class GUIObject
	{
	public:
		GUIObject();
		virtual ~GUIObject();
		
		virtual void Render(unsigned int VAO);
		virtual void Update();

		GUIObject* getParent();
		glm::mat4 getModel();


		Attribute& attr(const std::string& key)
		{
			return attributes[key];
		}

		/*template <typename T>
		T get(const std::string& key)
		{
			return attributes.get(key).value<T>();
		}*/


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
		void AddChild(GUIObject* obj);

		std::vector<GUIObject*> children;
		GUIObject* parent;
		glm::mat4 model;
		AttributeContainer attributes;

	private:
		static float vertices[12];
		static unsigned int indices[6];

		friend class Markup;
		friend class DOM;
	};
}

#endif // HI_GUI_OBJECT_H
