#ifndef HI_DOM_H
#define HI_DOM_H

#include <iostream>

#include "markup.h"
#include "style.h"

#include "markup_elements/div.h"

namespace higui
{
	class DOM
	{
	public:
		DOM(GLFWwindow* window, const std::string& markup_file, const std::string& style_file);

		void Render();
		void Delete();

		void MouseButtonCallback(int button, int action, int mods);
		void CursorPosCallback(double xpos, double ypos);
		void ScrollCallback(double xoffset, double yoffset);
		void FramebufferSizeCallback(int width, int height);
		void KeyCallback(int key, int scancode, int action, int mods);

		GLFWwindow* getGLFWwindow();

		Markup markup;
		ShaderRegistry shaders;
	

	private:
		static void MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
		static void FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
		static void KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);

		GLFWwindow* window;
		unsigned int VBO, VAO, EBO;
	};
}

#endif // HI_DOM_H
