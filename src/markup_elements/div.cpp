#include "markup_elements/div.h"

namespace higui
{
	namespace tag
	{
		Div::Div() : GUIObject(), alignment()
		{
			model = glm::mat4(1.0f);
		}

		void Div::Render(unsigned int VAO)
		{
			auto default_shader = Shader::get();
			default_shader->use();
			default_shader->setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			for (auto& child : children)
			{
				child->Render(VAO);
			}
		}

		void Div::Update()
		{
			if (parent)
			{
				model = parent->getModel();
			}
			else
			{
				model = glm::mat4(1.0f);
			}

			alignment = attribute_container.value<attribute::Alignment>();	// update alignment from container

			CalculateDock();												// calculate alignment

			for (auto& child : children)
			{
				child->Update();
			}
		}

		void Div::CalculateDock()
		{
			geometry3 g{ glm::vec3(0.0f), glm::vec3(1.0f) };

			switch (alignment.pos)
			{
			case Align::None:
				g.dim = glm::vec3(alignment.ratio);
				break;
			case Align::Top:
				g.pos.y = 1.0f - alignment.ratio;
				g.dim.y = alignment.ratio;
				break;
			case Align::Bottom:
				g.pos.y = -(1.0f - alignment.ratio);
				g.dim.y = alignment.ratio;
				break;
			case Align::Left:
				g.pos.x = -(1.0f - alignment.ratio);
				g.dim.x = alignment.ratio;
				break;
			case Align::Right:
				g.pos.x = 1.0f - alignment.ratio;
				g.dim.x = alignment.ratio;
				break;
			default:
				break;
			}

			model = glm::translate(model, g.pos);
			model = glm::scale(model, g.dim);
		}

		attribute::Alignment Div::getAlignment()
		{
			return alignment;
		}

		Align Div::getAlignPos()
		{
			return alignment.pos;
		}

		float Div::getAlignRatio()
		{
			return alignment.ratio;
		}

		void Div::setAlignment(attribute::Alignment alignment_)
		{
			alignment = alignment_;
		}

		void Div::setAlignPos(Align dock_pos)
		{
			alignment.pos = dock_pos;
		}

		void Div::setAlignRatio(float ratio)
		{
			alignment.ratio = ratio;
		}
	}
}