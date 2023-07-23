#ifndef DOM_H
#define DOM_H

#include "markup_parser.h"
#include "shader.h"

namespace higui
{
	class DOM
	{
	public:
		DOM(GLFWwindow* window, const std::string& markup_file, const std::string& style_file);

		// if your '.markup' '.style' files have the same name
		DOM(GLFWwindow* window, const std::string& name);

		void Render();
		void DisableGLBlending();
		void Delete();

		void MouseButtonCallback(int button, int action, int mods);
		void CursorPosCallback(double xpos, double ypos);
		void ScrollCallback(double xoffset, double yoffset);
		void FramebufferSizeCallback(int width, int height);
		void KeyCallback(int key, int scancode, int action, int mods);

		MarkupParser markup;
		ShaderManager shaders;
	

	private:
		static void MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
		static void FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
		static void KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);

		GLFWwindow* window;

		static float vertices[12];
		static unsigned int indices[6];
		unsigned int VBO, VAO, EBO;
	};
}

#endif // DOM_H
