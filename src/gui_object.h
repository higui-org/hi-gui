#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "custom_any.h"
#include "shader.h"

namespace higui
{
	class DOM;

	enum class ElementDock
	{
		none,
		top,
		bottom,
		left,
		right
	};

	class GUIObject
	{
	public:
		GUIObject();
		virtual ~GUIObject();
		
		void Render(unsigned int VAO);
		void CalculateDock();

		void setPadding(glm::vec4 padding);
		void setDock(ElementDock dock, float ratio = 0.25f);

		GUIObject* getParent();
		glm::vec4 getPadding();
		ElementDock getDock();

	private:
		GUIObject* parent;
		std::vector<GUIObject*> children;

		glm::mat4 model;
		glm::vec4 padding;

		ElementDock dock;
		float dock_ratio;

	protected:
		void AddChild(GUIObject* obj);

		AnyMap<std::string> properties;

		friend class MarkupParser;
	};
}

#endif // GUI_OBJECT_H
