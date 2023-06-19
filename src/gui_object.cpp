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

	void GUIObject::PrintInfo()
	{
		std::cout << "name: " << this->name << std::endl
			<< "padding: x: " << padding.x << ", y: " << padding.y << ", z: " << padding.z << ", w: " << padding.w << std::endl
			<< "margin: x: " << margin.x << ", y: " << margin.y << std::endl;
		for (auto& property : properties)
		{
			std::cout << "name: " << property.first << ", value: " << std::to_string(AnyCast<float>(property.second)) << std::endl << std::endl;
		}

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