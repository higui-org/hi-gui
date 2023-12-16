﻿#include "object.h"

namespace hi
{
	// class Transform. getters begin
	//
	// getters. normalized
	glm::vec3 Transform::getPos()
	{
		return glm::vec3(model[3]);
	}
	
	glm::vec3 Transform::getScale()
	{
		glm::vec3 scale;
		scale.x = glm::length(glm::vec3(model[0]));
		scale.y = glm::length(glm::vec3(model[1]));
		scale.z = glm::length(glm::vec3(model[2]));
		return scale;
	}

	glm::quat Transform::getRotation()
	{
		glm::vec3 scale = getScale();
		glm::mat3 rotation_mat(
			glm::vec3(model[0]) / scale.x,
			glm::vec3(model[1]) / scale.y,
			glm::vec3(model[2]) / scale.z);
		return glm::quat_cast(rotation_mat);
	}

	glm::mat4 Transform::getModel() 
	{ 
		return model; 
	}

	geometry<float, 3> Transform::getGeometry()
	{
		return {getPos(), getScale()};
	}

	void Transform::Translate(const glm::vec3& pos)
	{
		model = glm::translate(model, pos);
	}

	void Transform::Rotate(const glm::quat& rotation)
	{
		model = model * glm::mat4_cast(rotation);
	}

	void Transform::Scale(const glm::vec3& scale)
	{
		model = glm::scale(model, scale);
	}

	void Transform::Model(const glm::mat4& new_model)
	{
		model = new_model;
	}
	// getters. screen
	glm::vec3 Transform::getScreenPos(GLFWwindow* window)
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
		glm::vec3 scale = getScale();
		screen_сoords.x = (ndc.x + 1.0f - scale.x) / 2.0f * framebuff_w;
		screen_сoords.y = (1.0f - ndc.y - scale.y) / 2.0f * framebuff_h; // Y-coordinate is inverted
		screen_сoords.z = (ndc.z + 1.0f - scale.z) / 2.0f; // Z-coordinate for depth

		return screen_сoords;
	}

	glm::vec3 Transform::getScreenScale(GLFWwindow* window)
	{
		int framebuff_w, framebuff_h;
		glfwGetFramebufferSize(window, &framebuff_w, &framebuff_h);

		glm::vec3 volume = { framebuff_w, framebuff_h, 1.0f };
		return volume * getScale();
	}

	geometry<float, 3> Transform::getScreenGeometry(GLFWwindow* window)
	{
		return { getScreenPos(window), getScreenScale(window) };
	}
	// class Transform. getters end



	//
	// internal
	//
	namespace internal
	{

		void GUIObjectBase::Render(unsigned int VAO)
		{
			std::for_each(children.begin(), children.end(), [&](Widget child) {child->Render(VAO); });
		}

		void GUIObjectBase::Update()
		{
			std::for_each(children.begin(), children.end(), [&](Widget child) {child->Update(); });
		}

		void GUIObjectBase::ResetModel(
			const glm::vec3& position = glm::vec3(1.0f), 
			const glm::vec3& scale    = glm::vec3(1.0f),
			const glm::quat& rotation = glm::vec3(1.0f))
		{
			transform.Model(glm::mat4(1.0f));
			transform.Translate(position);
			transform.Rotate(rotation);
			transform.Scale(scale);
		}

		void GUIObjectBase::AddAttr(Attribute attribute)
		{
			
		}



		// events
		bool GUIObjectBase::OnCursorPos(double xpos, double ypos)
		{
			return false;
		}

		bool GUIObjectBase::OnMouseClick(int button, double xpos, double ypos)
		{
			return false;
		}



		// figure
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



		
		Widget GUIObjectBase::MouseIn(glm::vec2 point, GLFWwindow* win) {
			using geometry2 = geometry<float, 2>;

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
	}
}