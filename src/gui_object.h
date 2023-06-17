#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <iostream>

#include "custom_any.h"

namespace higui
{

	class GUIObject
	{
	public:
		GUIObject();
		~GUIObject();

		void setParent(GUIObject* parent);
		void setPadding(glm::vec4 padding);
		void setMargin(glm::vec4 margin);

		void render() { std::cout << "render " << name << std::endl; }

		GUIObject* getParent();
		glm::vec4 getPadding();
		glm::vec2 getMargin();

		std::string name;

	private:
		GUIObject* parent;
		std::vector<GUIObject*> children;

		glm::vec4 padding;
		glm::vec2 margin;

	protected:
		AnyMap<std::string> properties;

		static int i;

		friend class MarkupParser;
	};
}

#endif // GUI_OBJECT_H
