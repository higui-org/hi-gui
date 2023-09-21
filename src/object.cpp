#include "object.h"

namespace higui
{
	GUIObject::GUIObject()
	{
		parent = nullptr;
		model = glm::mat4(1.0f);
	}

	void GUIObject::Render(unsigned int VAO)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (auto& child : children)
		{
			child->Render(VAO);
		}
	}

	void GUIObject::Update()
	{
		for (auto& child : children)
		{
			child->Update();
		}
	}

	void GUIObject::AddChild(std::shared_ptr<GUIObject> obj)
	{
		children.push_back(obj);
	}

	std::shared_ptr<GUIObject> GUIObject::getParent()
	{
		return parent;
	}

	glm::mat4 GUIObject::getModel()
	{
		return model;
	}

	glm::vec2 GUIObject::Size(GLFWwindow* window)
	{
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return Size(w, h);
	}

	glm::vec2 GUIObject::Position(GLFWwindow* window)
	{
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return Position(w, h);
	}

	glm::vec4 GUIObject::Geometry(GLFWwindow* window)
	{
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return Geometry(w, h);
	}

	glm::vec2 GUIObject::Size(int framebuffer_width, int framebuffer_height)
	{
		float nw = model[0][0] - model[0][2];
		float nh = model[1][1] - model[0][1];

		float w = nw * framebuffer_width;
		float h = nh * framebuffer_height;

		return glm::vec2(w, h);
	}

	glm::vec2 GUIObject::Position(int framebuffer_width, int framebuffer_height)
	{
		float nx = model[3][0];
		float ny = model[1][1] + model[3][1];

		float x = nx * framebuffer_width;
		float y = (1.0f - ny) * (framebuffer_height / 2);

		return glm::vec2(x, y);
	}

	glm::vec4 GUIObject::Geometry(int w, int h)
	{
		return glm::vec4(Position(w, h), Size(w, h));
	}

	bool GUIObject::OnCursorPos(double xpos, double ypos) 
	{
		return false;
	}

	bool GUIObject::OnMouseClick(int button, double xpos, double ypos)
	{
		return false;
	}

	float GUIObject::vertices[12] = {
				-1.0f, 1.0f, 0.0f,  // upper left angle
				1.0f, 1.0f, 0.0f,   // upper right angle
				1.0f, -1.0f, 0.0f,  // lower right angle
				-1.0f, -1.0f, 0.0f  // lower left angle
	};

	unsigned int GUIObject::indices[6] = {
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
	};
}