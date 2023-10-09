#include "DOM.h"

namespace higui
{

	DOM::DOM(GLFWwindow* window, const std::string& markup_file, const std::string& style_file)
		: markup(markup_file)
	{
		this->window = window;
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
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			std::shared_ptr<internal::GUIObjectBase> obj = markup.central_object->children[0];
			attr::Alignment& align = obj->attr<attr::Alignment>("align");

			if (align.pos == Align::Top)
			{
				std::cout << "top!! i change it! " << std::endl;
				align.pos = Align::Bottom;

				std::cout << obj->attr<attr::Alignment>("align") << std::endl;
			}
			else
			{
				std::cout << "bottom>.. top now!" << std::endl;
				align.pos = Align::Top;
			}
			obj->Update();
		}
	}

	void DOM::CursorPosCallback(double xpos, double ypos)
	{

	}

	void DOM::ScrollCallback(double xoffset, double yoffset)
	{

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