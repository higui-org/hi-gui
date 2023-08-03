#include "markup_elements/div_element.h"

namespace higui
{
	DivElement::DivElement() : GUIObject()
	{
		model = glm::mat4(1.0f);
		dock = ElementDock::none;
		dock_ratio = 0.5f;
	}

	DivElement::~DivElement()
	{

	}

	void DivElement::Render(unsigned int VAO)
	{
		ShaderManager::shader("default")->use();
		ShaderManager::shader("default")->setMat4("model", model);
		GUIObject::Render(VAO);
	}

	void DivElement::Update()
	{
		model = glm::mat4(1.0f);
		CalculateDock();
		GUIObject::Update();
	}

	void DivElement::CalculateDock()
	{
		if (parent)
		{
			model = parent->getModel() * model;
		}

		char dock_char;
		try
		{
			dock_char = get("dock").at(0);
		}
		catch (std::exception e)
		{
			return;
		}

		try 
		{
			dock_ratio = get<float>("dock-ratio");
		}
		catch (std::exception e)
		{
			dock_ratio = 0.5f;
		}

		switch (dock_char)
		{
		case 'n':
			break;
		case 't':
			model = glm::translate(model, glm::vec3(0.0f, 1.0f - dock_ratio, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock_ratio, 1.0f));
			break;
		case 'b':
			model = glm::translate(model, glm::vec3(0.0f, dock_ratio - 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, dock_ratio, 1.0f));
			break;
		case 'l':
			model = glm::translate(model, glm::vec3(dock_ratio - 1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock_ratio, 1.0f, 1.0f));
			break;
		case 'r':
			model = glm::translate(model, glm::vec3(1.0f - dock_ratio, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(dock_ratio, 1.0f, 1.0f));
			break;
		default:
			break;
		}
	}

	ElementDock DivElement::getDock()
	{
		return dock;
	}

	float DivElement::getDockRatio()
	{
		return dock_ratio;
	}

	void DivElement::setDock(ElementDock dock_, float ratio)
	{
		switch (dock_)
		{
		case ElementDock::none:
			set("none", ToString(ratio));
			break;
		case ElementDock::top:
			set("top", ToString(ratio));
			break;
		case ElementDock::bottom:
			set("bottom", ToString(ratio));
			break;
		case ElementDock::left:
			set("left", ToString(ratio));
			break;
		case ElementDock::right:
			set("right", ToString(ratio));
			break;
		default:
			break;
		}
		Update();
	}

	void DivElement::setDockRatio(float ratio)
	{
		setDock(dock, ratio);
	}
}