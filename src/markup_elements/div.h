#ifndef DIV_H
#define DIV_H

#include "object.h"

namespace higui 
{
	class DivTag : public GUIObject<DivTag>
	{
	public:
		DivTag();
		~DivTag();

		// overrided and derived from GUIObject
		void Render(unsigned int VAO) override;
		void Update() override;

		void CalculateDock();

		attr::Alignment getAlignment();
		Align getAlignPos();
		float getAlignRatio();

		void setAlignment(attr::Alignment alignment);
		void setAlignPos(Align align);
		void setAlignRatio(float ratio);

	protected:
		attr::Alignment alignment;
	};
}

#endif // DIV_H
