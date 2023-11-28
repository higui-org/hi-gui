#include "component.h"

namespace higui
{
	Event::Event(GLFWwindow* win) : window(win) {}
	{
		glfwSetWindowUserPointer(window, this);

		glfwSetKeyCallback(window, DOM::KeyCallbackWrapper);
		glfwSetScrollCallback(window, DOM::ScrollCallbackWrapper);
		glfwSetCursorPosCallback(window, DOM::CursorPosCallbackWrapper);
		glfwSetMouseButtonCallback(window, DOM::MouseButtonCallbackWrapper);
		glfwSetFramebufferSizeCallback(window, DOM::FramebufferSizeCallbackWrapper);
	}

	void Event::MouseButtonCallback(int button, int action, int mods)
	{

	}

	void Event::CursorPosCallback(double xpos, double ypos)
	{
		hiObject temp = central_object->MouseIn({ xpos, ypos }, event.window.window_ptr);
		if (focused_obj != temp)
		{
			focused_obj = temp;
			system("cls");
			std::cout << focused_obj << std::endl;
		}
	}

	void Event::ScrollCallback(double xoffset, double yoffset)
	{
		int x, y;
		glfwGetFramebufferSize(event.window.window_ptr, &x, &y);
		Attribute& align = (*focused_obj)("align");
		if (yoffset > 0)
		{
			align.value<attribute::Alignment>().ratio += 0.03f;
		}
		else if (yoffset < 0)
		{
			align.ratio -= 0.03f;
		}
		focused_obj->Update();
		system("cls");
		std::cout << focused_obj << std::endl;
	}

	void Event::FramebufferSizeCallback(int width, int height)
	{
		glViewport(0, 0, width, height);

		central_object->Update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();

		glfwSwapBuffers(event.window.window_ptr);
		glfwPollEvents();
	}

	void Event::KeyCallback(int key, int scancode, int action, int mods)
	{

	}



	void Event::MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->MouseButtonCallback(button, action, mods);
		}
	}

	void Event::CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->CursorPosCallback(xpos, ypos);
		}
	}

	void Event::ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->ScrollCallback(xoffset, yoffset);
		}
	}

	void Event::FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->FramebufferSizeCallback(width, height);
		}
	}

	void Event::KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->KeyCallback(key, scancode, action, mods);
		}
	}
}