#include "gui_object.h"

namespace higui
{

	int GUIObject::i = 0;

	GUIObject::GUIObject()
	{
		this->name = "sOME GUIObject" + std::to_string(i);
		i++;
		padding = glm::vec4(25, 25, 25, 25);
		margin = glm::vec2(0, 0);
	}

	GUIObject::~GUIObject()
	{
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