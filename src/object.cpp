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

		void GUIObjectBase::ResetModel(
			const glm::vec3& position = glm::vec3(1.0f), 
			const glm::vec3& scale    = glm::vec3(1.0f),
			const glm::quat& rotation = glm::vec3(1.0f))
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = model * glm::toMat4(rotation);
			model = glm::scale(model, scale);
		}

		glm::vec3 GUIObjectBase::Position()
		{
			return glm::vec3(model[3]);
		}

		glm::vec3 GUIObjectBase::Scale()
		{
			glm::vec3 scale;
			scale.x = glm::length(glm::vec3(model[0]));
			scale.y = glm::length(glm::vec3(model[1]));
			scale.z = glm::length(glm::vec3(model[2]));
			return scale;
		}

		glm::quat GUIObjectBase::Rotation()
		{
			glm::vec3 scale = Scale();
			glm::mat3 rotation_mat(
				glm::vec3(model[0]) / scale.x,
				glm::vec3(model[1]) / scale.y,
				glm::vec3(model[2]) / scale.z);
			return glm::quat_cast(rotation_mat);
		}

		geometry3 GUIObjectBase::Geometry()
		{
			return { Position(), Scale() };
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

		std::shared_ptr<GUIObjectBase> GUIObjectBase::MouseIn(glm::vec2 point, GLFWwindow* win) {
			std::shared_ptr<GUIObjectBase> closest_object = nullptr;
			std::shared_ptr<GUIObjectBase> intersected_object = nullptr;
			float closest_distance = std::numeric_limits<float>::max();

			std::function<void(std::shared_ptr<GUIObjectBase>, const geometry2&)> getObjectWhereMouse =
				[&](std::shared_ptr<GUIObjectBase> obj, const geometry2& parent_geometry) {
				geometry2 obj_geometry = geometry2{ obj->ScreenGeometry(win) };

				// Перевіряємо, чи точка лежить всередині геометрії поточного об'єкта
				if (isPointInsideGeometry(point, obj_geometry)) {
					float distance = DistanceToGeometryCenter(point, obj_geometry);
					if (distance < closest_distance) {
						closest_distance = distance;
						closest_object = obj;
					}
				}

				// Перевіряємо перетин з батьківською геометрією
				geometry2 intersect_geometry = getGeometriesIntersect(parent_geometry, obj_geometry);
				if (intersect_geometry.dim.x > 0 && intersect_geometry.dim.y > 0) { // Якщо перетин існує
					if (isPointInsideGeometry(point, intersect_geometry)) {
						intersected_object = obj; // Точка лежить на перетині
					}
				}

				for (auto& child : obj->children) {
					getObjectWhereMouse(child, obj_geometry);
				}
			};

			geometry2 root_geometry = geometry2{ ScreenGeometry(win) };
			if (parent)
			{
				root_geometry = parent->ScreenGeometry(win);
			}
			getObjectWhereMouse(shared_from_this(), root_geometry);

			// Якщо точка лежить на перетині дочірнього об'єкта, повертаємо дочірній об'єкт
			return intersected_object ? intersected_object : closest_object;
		}

		glm::vec3 GUIObjectBase::ScreenCoords(GLFWwindow* window)
		{
			int framebuff_w, framebuff_h;
			glfwGetFramebufferSize(window, &framebuff_w, &framebuff_h);

			// Transformation of coordinates into normalized camera space
			glm::vec4 clip_coords = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			// Checking for correct conversion
			if (clip_coords.w == 0.0f) {
				return glm::vec3(0.0f);
			}

			// Conversion from normalized camera space to normalized device coordinates
			glm::vec3 ndc = glm::vec3(clip_coords) / clip_coords.w;

			// Conversion from normalized device coordinates to pixel coordinates
			glm::vec3 screen_сoords;
			glm::vec3 scale = Scale();
			screen_сoords.x = (ndc.x + 1.0f - scale.x) / 2.0f * framebuff_w;
			screen_сoords.y = (1.0f - ndc.y - scale.y) / 2.0f * framebuff_h; // Y-coordinate is inverted
			screen_сoords.z = (ndc.z + 1.0f - scale.z) / 2.0f; // Z-coordinate for depth

			return screen_сoords;
		}

		glm::vec3 GUIObjectBase::ScreenDimensions(GLFWwindow* window)
		{
			int framebuff_w, framebuff_h;
			glfwGetFramebufferSize(window, &framebuff_w, &framebuff_h);

			glm::vec3 volume = { framebuff_w, framebuff_h, 1.0f };
			return volume * Scale();
		}

		geometry3 GUIObjectBase::ScreenGeometry(GLFWwindow* window)
		{
			return { ScreenCoords(window), ScreenDimensions(window) };
		}
	}
}