#include "object.h"

namespace higui
{
	namespace internal
	{

		void GUIObject::Render(unsigned int VAO)
		{
			for (auto& c : components) c.Render();
			std::for_each(children.begin(), children.end(), [&](hiObject child) {child->Render(VAO); });
		}

		void GUIObject::Update()
		{
			std::for_each(component.begin(), component.end(), [&](Component comp) {comp.Update(this); });
			std::for_each(children.begin(), children.end(), [&](hiObject child) {child->Update(); });
		}

		void GUIObject::ResetModel(
			const glm::vec3& position = glm::vec3(1.0f), 
			const glm::vec3& scale    = glm::vec3(1.0f),
			const glm::quat& rotation = glm::vec3(1.0f))
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = model * glm::toMat4(rotation);
			model = glm::scale(model, scale);
		}

		void GUIObject::AddAttr(Attribute attribute)
		{
			
		}



		// normalized
		glm::vec3 GUIObject::Position()
		{
			return glm::vec3(model[3]);
		}
		
		glm::vec3 GUIObject::Scale()
		{
			glm::vec3 scale;
			scale.x = glm::length(glm::vec3(model[0]));
			scale.y = glm::length(glm::vec3(model[1]));
			scale.z = glm::length(glm::vec3(model[2]));
			return scale;
		}
		
		glm::quat GUIObject::Rotation()
		{
			glm::vec3 scale = Scale();
			glm::mat3 rotation_mat(
				glm::vec3(model[0]) / scale.x,
				glm::vec3(model[1]) / scale.y,
				glm::vec3(model[2]) / scale.z);
			return glm::quat_cast(rotation_mat);
		}

		geometry3 GUIObject::Geometry()
		{
			return { Position(), Scale() };
		}



		// events
		bool GUIObject::OnCursorPos(double xpos, double ypos)
		{
			return false;
		}

		bool GUIObject::OnMouseClick(int button, double xpos, double ypos)
		{
			return false;
		}



		// figure
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



		
		hiObject GUIObject::MouseIn(glm::vec2 point, GLFWwindow* win) {
			hiObject closest_object = nullptr;
			hiObject intersected_object = nullptr;
			float closest_distance = std::numeric_limits<float>::max();

			std::function<void(hiObject, const geometry2&)> getObjectWhereMouse = 
				[&](hiObject obj, const geometry2& parent_geometry)
			{
				geometry2 obj_geometry = geometry2{ obj->ScreenGeometry(win) };

				// check whether the point lies inside the geometry of the current object
				if (isPointInsideGeometry(point, obj_geometry)) {
					float distance = DistanceToGeometryCenter(point, obj_geometry);
					if (distance < closest_distance) {
						closest_distance = distance;
						closest_object = obj;
					}
				}

				// check the intersection with the parent geometry
				geometry2 intersect_geometry = GeometryIntersection(parent_geometry, obj_geometry);
				if (intersect_geometry.dim.x > 0 && intersect_geometry.dim.y > 0) { // If the intersection exists
					if (isPointInsideGeometry(point, intersect_geometry)) {
						intersected_object = obj; // The point lies on the intersection
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

			// If the point lies at the intersection of the child object, return the child object
			return intersected_object ? intersected_object : closest_object;
		}




		// screen
		glm::vec3 GUIObject::ScreenCoords(GLFWwindow* window)
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

		glm::vec3 GUIObject::ScreenDimensions(GLFWwindow* window)
		{
			int framebuff_w, framebuff_h;
			glfwGetFramebufferSize(window, &framebuff_w, &framebuff_h);

			glm::vec3 volume = { framebuff_w, framebuff_h, 1.0f };
			return volume * Scale();
		}

		geometry3 GUIObject::ScreenGeometry(GLFWwindow* window)
		{
			return { ScreenCoords(window), ScreenDimensions(window) };
		}

		std::ostream& operator<<(std::ostream& os, const hiObject& obj)
		{
			os << "====================" << std::endl;
			os << "obj: " << reinterpret_cast<int>(&obj) << std::endl;
			os << obj->Geometry() << std::endl;
			std::for_each(obj->container.begin(), obj->container.end(), [&](Attribute attr) { os << attr << std::endl; });
			os << "====================" << std::endl;
			return os;
		}
	}
}