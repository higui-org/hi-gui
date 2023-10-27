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
			model = glm::mat4(1.0f);
			alignment = attribute_container.value<attribute::Alignment>();

			CalculateDock();

			for (auto& child : children)
			{
				child->Update();
			}
		}

		void Div::CalculateDock() 
		{
			if (parent)
			{
				model = parent->getModel() * model;
			}

			glm::vec3 t(0.0f, 0.0f, 0.0f); // stand for translation
			glm::vec3 s(1.0f, 1.0f, 1.0f); // stand for scaling

			switch (alignment.pos)
			{
			case Align::None:
				s = glm::vec3(alignment.ratio);
				break;
			case Align::Top:
				t.y = 1.0f - alignment.ratio;
				s.y = alignment.ratio;
				break;
			case Align::Bottom:
				t.y = alignment.ratio - 1.0f;
				s.y = alignment.ratio;
				break;
			case Align::Left:
				t.x = alignment.ratio - 1.0f;
				s.x = alignment.ratio;
				break;
			case Align::Right:
				t.x = 1.0f - alignment.ratio;
				s.x = alignment.ratio;
				break;
			default:
				break;
			}

			model = glm::translate(model, t);
			model = glm::scale(model, s);
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