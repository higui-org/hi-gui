#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

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

		void set(std::string attribute, std::string value);

		template <typename T> 
		T get(std::string attribute) {
			if (properties.find(attribute) == properties.end())
				throw std::runtime_error("Cannot find the attribute in GUI object");
			return string::To<T>(properties[attribute]);
		}
		std::string get(std::string attribute);

		GUIObject* getParent();
		glm::mat4 getModel();

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

		GUIObject* parent;
		std::vector<GUIObject*> children;

		glm::mat4 model;

		std::unordered_map<std::string, std::string> properties;

	private:
		static float vertices[12];
		static unsigned int indices[6];

		friend class MarkupParser;
		friend class DOM;
	};
}

#endif // GUI_OBJECT_H
