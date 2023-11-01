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
			glfwGetFramebufferSize(window, &w, &h); // Отримуємо розміри буфера кадру

			// Обчислюємо розміри об'єкта у нормалізованих координатах пристрою (NDC)
			float ndcWidth = (model[0][0] - model[0][2]) * 2.0f; // множимо на 2.0f, оскільки NDC змінюються від -1 до 1
			float ndcHeight = (model[1][1] - model[1][2]) * 2.0f; // множимо на 2.0f з тієї ж причини

			// Переводимо NDC в пікселі
			float pixelWidth = ndcWidth * w * 0.5f; // ділимо на 0.5f, оскільки розмір екрана - це від 0 до w
			float pixelHeight = ndcHeight * h * 0.5f; // аналогічно для висоти

			return glm::vec2(pixelWidth, pixelHeight);
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
			float ny =  model[3][1] * 2.0f;

			/*switch (attr<attribute::Alignment>("align").pos)
			{
			case Align::Left:
				nx += 1.0f - model[0][0] - model[0][2];
				break;
			case Align::Top:
				ny += 1.0f - model[1][1] - model[0][1];
				break;
			case Align::Bottom:
				ny -= 1.0f - model[1][1] - model[0][1];
				break;
			case Align::None:
				nx += 1.0f - model[0][0] - model[0][2];
				ny -= 1.0f - model[1][1] - model[0][1];
				break;
			default:
				break;
			}*/

			glm::vec2 vec;
			vec.x = (nx) * (framebuffer_w / 2);
			vec.y = (ny) * (framebuffer_h / 2);

			if (parent)
				vec += parent->pos(window);

			return vec;
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