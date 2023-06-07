#include "gui_object.h"

GUIObject::GUIObject()
{
	parent = nullptr;
	padding = glm::vec4(25, 25, 25, 25);
	margin = glm::vec2(0, 0);
	dock = ObjectDock::none;
}

GUIObject::~GUIObject() 
{
}

void GUIObject::useDock()
{
	if (parent)
	{
		
	}
}

void GUIObject::calculatePos()
{

}

void GUIObject::setParent(GUIObject* parent)
{
	if (parent) this->parent = parent;
}

void GUIObject::setDock(ObjectDock dock)
{
	this->dock = dock;
}

void GUIObject::setPadding(glm::vec4 padding)
{
	this->padding = padding;
}

void GUIObject::setMargin(glm::vec4 margin)
{
	glm::vec2 temp = { margin.x - margin.z, margin.y - margin.w};
	this->margin = temp;
}


GUIObject* GUIObject::getParent()
{
	return parent;
}

ObjectDock GUIObject::getDock()
{
	return dock;
}

glm::vec4 GUIObject::getPadding()
{
	return padding;
}

glm::vec2 GUIObject::getMargin()
{
	return margin;
}