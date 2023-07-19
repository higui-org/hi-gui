#include "gui_object.h"

namespace higui
{
	GUIObject::GUIObject()
	{
		parent = nullptr;
		model = glm::mat4(1.0f);
	}

	GUIObject::~GUIObject()
	{
		for (auto child : children) 
		{
			delete child;
		}
	}

	void GUIObject::Render(unsigned int VAO)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (auto& child : children)
		{
			child->Render(VAO);
		}
	}

	void GUIObject::Update()
	{
		for (auto& child : children)
		{
			child->Update();
		}
	}

	void GUIObject::AddChild(GUIObject* obj)
	{
		children.push_back(obj);
	}

	void GUIObject::set(std::string attribute, std::string value)
	{
		properties[attribute] = value;
	}

	std::string GUIObject::get(std::string attribute)
	{
		if (properties.find(attribute) == properties.end())
			return "";
		return properties[attribute];
	}

	GUIObject* GUIObject::getParent()
	{
		return parent;
	}

	glm::mat4 GUIObject::getModel()
	{
		return model;
	}
}