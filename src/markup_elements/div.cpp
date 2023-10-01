#include "markup_elements/div.h"

namespace higui
{
	DivTag::DivTag() : GUIObject(), dock()
	{
		model = glm::mat4(1.0f);
	}

	DivTag::~DivTag()
	{

	}

	void DivTag::Render(unsigned int VAO)
	{
		auto default_shader = Shader::get("default");
		default_shader->use();
		default_shader->setMat4("model", model);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		for (auto& child : children)
		{
			child->Render(VAO);
		}
	}

	void DivTag::Update()
	{
		model = glm::mat4(1.0f);

		CalculateDock();
		
		for (auto& child : children)
		{
			child->Update();
		}
	}

	void DivTag::CalculateDock()
	{
		if (parent)
		{
			model = parent->getModel() * model;
		}
		switch (dock.pos)
		{
		case DockPosition::None:
			break;
		case DockPosition::Top:
			model = glm::translate(model, glm::vec3(0.0f, 1.0f - dock.ratio, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock.ratio, 1.0f));
			break;
		case DockPosition::Bottom:
			model = glm::translate(model, glm::vec3(0.0f, dock.ratio - 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock.ratio, 1.0f));
			break;
		case DockPosition::Left:
			model = glm::translate(model, glm::vec3(dock.ratio - 1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock.ratio, 1.0f, 1.0f));
			break;
		case DockPosition::Right:
			model = glm::translate(model, glm::vec3(1.0f - dock.ratio, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock.ratio, 1.0f, 1.0f));
			break;
		default:
			break;
		}
	}

	AttributeDock DivTag::getDock()
	{
		return dock;
	}

	DockPosition DivTag::getDockPosition()
	{
		return dock.pos;
	}

	float DivTag::getDockRatio()
	{
		return dock.ratio;
	}

	void DivTag::setDock(AttributeDock dock_)
	{
		dock = dock_;
	}

	void DivTag::setDockPosition(DockPosition dock_pos)
	{
		dock.pos = dock_pos;
	}

	void DivTag::setDockRatio(float ratio)
	{
		dock.ratio = ratio;
	}
}