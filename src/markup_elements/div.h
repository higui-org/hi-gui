#ifndef Hi_DIV_H
#define Hi_DIV_H

#include "object.h"

namespace higui 
{
	namespace tag
	{
		class Div : public GUIObject<Div>
		{
		public:
			Div();

			// overrided and derived from GUIObject
			void Render(unsigned int VAO) override;
			void Update() override;

			void CalculateDock();

			attribute::Alignment getAlignment();
			Align getAlignPos();
			float getAlignRatio();

			void setAlignment(attribute::Alignment alignment);
			void setAlignPos(Align align);
			void setAlignRatio(float ratio);

		protected:
			attribute::Alignment alignment;
		};
	}
}

#endif // Hi_DIV_H
