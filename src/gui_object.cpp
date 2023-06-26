#include "gui_object.h"

namespace higui
{
	GUIObject::GUIObject()
	{
		name = "gui-object";
		padding = glm::vec4(25, 25, 25, 25);
		margin = glm::vec2(0, 0);
	}

	GUIObject::~GUIObject()
	{
		std::cout << "gui object deleted" << std::endl;
		for (auto child : children) {
			delete child;
		}
	}

	void GUIObject::Render()
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GUIObject::AddChild(GUIObject* obj)
	{
		children.push_back(obj);
	}

	void GUIObject::setParent(GUIObject* parent)
	{
		if (parent) this->parent = parent;
	}

	void GUIObject::setPadding(glm::vec4 padding)
	{
		this->padding = padding;
	}

	void GUIObject::setMargin(glm::vec4 margin)
	{
		glm::vec2 temp = { margin.x - margin.z, margin.y - margin.w };
		this->margin = temp;
	}


	GUIObject* GUIObject::getParent()
	{
		return parent;
	}

	glm::vec4 GUIObject::getPadding()
	{
		return padding;
	}

	glm::vec2 GUIObject::getMargin()
	{
		return margin;
	}

}