#include "DOM.h"

namespace higui
{
	bool DOM::resizeFlag = false;
	std::chrono::steady_clock::time_point DOM::lastResizeTime;

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
		if (action == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			std::shared_ptr<internal::GUIObjectBase> obj = markup.central_object->children[0]->MouseIn({ x, y }, window);
			if (obj)
			{
				geometry3 geo = obj->ScreenGeometry(window);
				attribute::Alignment& align = obj->attr<attribute::Alignment>();

				std::cout << "=== clicked === obj: " << obj << " ===" << std::endl;
				std::cout << "x: " << geo.pos.x << " \t\t y: " << geo.pos.y << " \t\t z: " << geo.pos.z << std::endl;
				std::cout << "w: " << geo.dim.x << " \t\t h: " << geo.dim.y << " \t z: " << geo.dim.z << std::endl;
				std::cout << "\t align: \t" << align << std::endl;
				std::cout << "====================" << std::endl;



				if (button == GLFW_MOUSE_BUTTON_1)
				{
					align.ratio -= 0.05f;
				}
				else if (button == GLFW_MOUSE_BUTTON_2)
				{
					align.ratio += 0.05f;
				}

				obj->Update();
			}
		}

		//if (obj)
		//{
		//	if (button == GLFW_MOUSE_BUTTON_1)
		//	{
		//		std::cout << "+" << std::endl;
		//		obj->attr<attribute::Alignment>().ratio += 0.03f;
		//	}
		//	if (button == GLFW_MOUSE_BUTTON_2)
		//	{
		//		std::cout << "-" << std::endl;
		//		obj->attr<attribute::Alignment>().ratio -= 0.03f;
		//	}

		//	geometry3 obj_geo = obj->ScreenGeometry(window);
		//	std::cout << "obj: " << obj << ", x: " << obj_geo.pos.x << ", y: " << obj_geo.pos.y << ", w: " << obj_geo.dim.x << ", h: " << obj_geo.dim.y << std::endl;
		//	for (auto child : obj->children)
		//	{
		//		geometry3 child_geo = child->ScreenGeometry(window);

		//		std::cout << "obj: " << child << ", x: " << child_geo.pos.x << ", y: " << child_geo.pos.y << ", w: " << child_geo.dim.x << ", h: " << child_geo.dim.y << std::endl;
		//	}

			// obj->Update();
		// }
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