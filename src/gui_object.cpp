#include "gui_object.h"

namespace higui
{
	GUIObject::GUIObject()
	{
		parent = nullptr;
		padding = glm::vec4(25, 25, 25, 25);
		model = glm::mat4(1.0f);

		dock = ElementDock::right;
		dock_ratio = 0.5f;
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
		model = glm::mat4(2.0f);
		CalculateDock();
		ShaderManager::shader("default")->use();
		ShaderManager::shader("default")->setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GUIObject::CalculateDock()
	{
		switch (dock)
		{
		case ElementDock::none:
			break;
		case ElementDock::top:
			model = glm::translate(model, glm::vec3(0.0f, 1.0f - dock_ratio, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock_ratio, 1.0f));
			model = glm::scale(model, glm::vec3(0.95f));
			break;
		case ElementDock::bottom:
			model = glm::translate(model, glm::vec3(0.0f, dock_ratio - 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock_ratio, 1.0f));
			model = glm::scale(model, glm::vec3(0.95f));
			break;
		case ElementDock::left:
			model = glm::translate(model, glm::vec3(dock_ratio - 1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock_ratio, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.95f));
			break;
		case ElementDock::right:
			model = glm::translate(model, glm::vec3(1.0f - dock_ratio, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock_ratio, 1.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.95f));
			break;
		default:
			break;
		}
	}

	void GUIObject::AddChild(GUIObject* obj)
	{
		children.push_back(obj);
	}

	void GUIObject::setPadding(glm::vec4 padding)
	{
		this->padding = padding;
	}

	void GUIObject::setDock(ElementDock dock, float ratio)
	{
		this->dock = dock;
		this->dock_ratio = ratio;
	}

	GUIObject* GUIObject::getParent()
	{
		return parent;
	}

	glm::vec4 GUIObject::getPadding()
	{
		return padding;
	}

	ElementDock GUIObject::getDock()
	{
		return dock;
	}
}