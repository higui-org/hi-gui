#include "markup_elements/div.h"

namespace higui
{
	DivTag::DivTag() : GUIObject()
	{
		model = glm::mat4(1.0f);
		dock = ElementDock::none;
		dock_ratio = 0.5f;
	}

	DivTag::~DivTag()
	{

	}

	void DivTag::Render(unsigned int VAO)
	{
		ShaderRegistry::shader("default")->use();
		ShaderRegistry::shader("default")->setMat4("model", model);
		GUIObject::Render(VAO);
	}

	void DivTag::Update()
	{
		model = glm::mat4(1.0f);
		CalculateDock();
		GUIObject::Update();
	}

	void DivTag::CalculateDock()
	{
		if (parent)
		{
			model = parent->getModel() * model;
		}

		char dock_char;
		try
		{
			//dock_char = get<std::string>("dock").at(0);
		}
		catch (std::exception)
		{
			setDock(ElementDock::none);
			dock_char = 'n';
		}

		try 
		{
			//dock_ratio = get<float>("dock-ratio");
		}
		catch (std::exception)
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

	ElementDock DivTag::getDock()
	{
		return dock;
	}

	float DivTag::getDockRatio()
	{
		return dock_ratio;
	}

	void DivTag::setDock(ElementDock dock_)
	{
		switch (dock_)
		{
		case ElementDock::none:
			attr("dock") = "none";
			break;
		case ElementDock::top:
			attr("dock") = "top";
			break;
		case ElementDock::bottom:
			attr("dock") = "bottom";
			break;
		case ElementDock::left:
			attr("dock") = "left";
			break;
		case ElementDock::right:
			attr("dock") = "right";
			break;
		default:
			break;
		}
	}

	void DivTag::setDockRatio(float ratio)
	{
		attr("dock-ratio") = ratio;
	}
}