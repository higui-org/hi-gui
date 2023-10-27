#include "object.h"

namespace higui
{
	namespace internal
	{
		void GUIObjectBase::Render(unsigned int VAO)
		{
			for (auto& child : children)
			{
				child->Render(VAO);
			}
		}

		void GUIObjectBase::Update()
		{
			for (auto& child : children)
			{
				child->Update();
			}
		}

		glm::vec2 GUIObjectBase::size(GLFWwindow* window)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);

			float nw = model[0][0] - model[0][2];
			float nh = model[1][1] - model[0][1];

			return glm::vec2(nw * w, nh * h);
		}

		void printMatrix(const glm::mat4 &matrix) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << matrix[i][j] << " ";
				}
				std::cout << std::endl;
			}
		}

		glm::vec2 GUIObjectBase::pos(GLFWwindow* window)
		{
			
			int framebuffer_w, framebuffer_h;
			glfwGetFramebufferSize(window, &framebuffer_w, &framebuffer_h);

			float nx = model[3][0];
			float ny = model[1][1];

			if (attr<attribute::Alignment>("align").pos == Align::Left)
			{
				nx += 1.0f - model[0][0] - model[0][2];
			}
			else if (attr<attribute::Alignment>("align").pos == Align::Top)
			{
				ny += 1.0f - model[1][1] - model[0][1];
			}
			else if (attr<attribute::Alignment>("align").pos == Align::Bottom)
			{
				ny -= 1.0f - model[1][1] - model[0][1];
			}

			float x = (nx) * framebuffer_w;
			float y = (1.0f - ny) * (framebuffer_h / 2);

			return glm::vec2(x, y);
		}

		glm::vec4 GUIObjectBase::geometry(GLFWwindow* window)
		{
			return glm::vec4(pos(window), size(window));
		}

		bool GUIObjectBase::OnCursorPos(double xpos, double ypos) 
		{
			return false;
		}

		bool GUIObjectBase::OnMouseClick(int button, double xpos, double ypos)
		{
			return false;
		}

		float GUIObjectBase::vertices[12] = {
					-1.0f, 1.0f, 0.0f,  // upper left angle
					1.0f, 1.0f, 0.0f,   // upper right angle
					1.0f, -1.0f, 0.0f,  // lower right angle
					-1.0f, -1.0f, 0.0f  // lower left angle
		};

		unsigned int GUIObjectBase::indices[6] = {
					0, 1, 3, // first triangle
					1, 2, 3  // second triangle
		};

		std::shared_ptr<GUIObjectBase> GUIObjectBase::Hover(glm::vec2 point, GLFWwindow* win)
		{
			if (internal::isPointInsideRect(point, geometry(win)))
			{
				for (auto& child : children)
				{
					std::shared_ptr<GUIObjectBase> hovered_child = nullptr;
					hovered_child = child->Hover(point, win);
					if (hovered_child)
					{
						return hovered_child;
					}
				}
				return shared_from_this();
			}
			return nullptr;
		}
	}
}