#include "markup_elements/div.h"

namespace higui
{
	DivTag::DivTag() : GUIObject(), alignment()
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
		switch (alignment.pos)
		{
		case Align::None:
			break;
		case Align::Top:
			model = glm::translate(model, glm::vec3(0.0f, 1.0f - alignment.ratio, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, alignment.ratio, 1.0f));
			break;
		case Align::Bottom:
			model = glm::translate(model, glm::vec3(0.0f, alignment.ratio - 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, alignment.ratio, 1.0f));
			break;
		case Align::Left:
			model = glm::translate(model, glm::vec3(alignment.ratio - 1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(alignment.ratio, 1.0f, 1.0f));
			break;
		case Align::Right:
			model = glm::translate(model, glm::vec3(1.0f - alignment.ratio, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(alignment.ratio, 1.0f, 1.0f));
			break;
		default:
			break;
		}
	}

	attr::Alignment DivTag::getAlignment()
	{
		return alignment;
	}

	Align DivTag::getAlignPos()
	{
		return alignment.pos;
	}

	float DivTag::getAlignRatio()
	{
		return alignment.ratio;
	}

	void DivTag::setAlignment(attr::Alignment alignment_)
	{
		alignment = alignment_;
	}

	void DivTag::setAlignPos(Align dock_pos)
	{
		alignment.pos = dock_pos;
	}

	void DivTag::setAlignRatio(float ratio)
	{
		alignment.ratio = ratio;
	}
}