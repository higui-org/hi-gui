#include "DOM.h"

namespace higui
{
	bool DOM::resizeFlag = false;
	std::chrono::steady_clock::time_point DOM::lastResizeTime;
	GUIObjectPtr DOM::obj = nullptr;

	DOM::DOM(GLFWwindow* window, const std::string& markup_file, const std::string& style_file)
		: markup(markup_file), window(window)
	{
		obj = nullptr;
		glfwSetWindowUserPointer(window, this);
		
		glfwSetKeyCallback(window, DOM::KeyCallbackWrapper);
		glfwSetScrollCallback(window, DOM::ScrollCallbackWrapper);
		glfwSetCursorPosCallback(window, DOM::CursorPosCallbackWrapper);
		glfwSetMouseButtonCallback(window, DOM::MouseButtonCallbackWrapper);
		glfwSetFramebufferSizeCallback(window, DOM::FramebufferSizeCallbackWrapper);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(internal::GUIObjectBase::vertices), internal::GUIObjectBase::vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(internal::GUIObjectBase::indices), internal::GUIObjectBase::indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void DOM::Render()
	{
		markup.central_object->Render(VAO);
	}

	void DOM::Delete()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void DOM::MouseButtonCallback(int button, int action, int mods)
	{
		
	}

	void DOM::CursorPosCallback(double xpos, double ypos)
	{
		GUIObjectPtr temp = markup.central_object->MouseIn({ xpos, ypos }, window);
		if (obj != temp)
		{
			obj = temp;
			system("cls");
			std::cout << obj << std::endl;
		}
	}

	void DOM::ScrollCallback(double xoffset, double yoffset)
	{
		int x, y;
		glfwGetFramebufferSize(window, &x, &y);
		attribute::Alignment& align = obj->attr<attribute::Alignment>();
		if (yoffset > 0)
		{
			align.ratio += 0.03f;
		}
		else if (yoffset < 0)
		{
			align.ratio -= 0.03f;
		}
		obj->Update();
		system("cls");
		std::cout << obj << std::endl;
	}

	void DOM::FramebufferSizeCallback(int width, int height)
	{
		glViewport(0, 0, width, height);

		markup.central_object->Update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void DOM::KeyCallback(int key, int scancode, int action, int mods)
	{

	}



	void DOM::MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->MouseButtonCallback(button, action, mods);
		}
	}

	void DOM::CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->CursorPosCallback(xpos, ypos);
		}
	}

	void DOM::ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->ScrollCallback(xoffset, yoffset);
		}
	}

	void DOM::FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->FramebufferSizeCallback(width, height);
		}
	}

	void DOM::KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		DOM* dom = static_cast<DOM*>(glfwGetWindowUserPointer(window));
		if (dom)
		{
			dom->KeyCallback(key, scancode, action, mods);
		}
	}

	GLFWwindow* DOM::getGLFWwindow()
	{
		return window;
	}
}