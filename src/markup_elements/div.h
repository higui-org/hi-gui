#ifndef DIV_H
#define DIV_H

#include "object.h"

namespace higui 
{
	class DivTag : public GUIObject
	{
	public:
		DivTag();
		~DivTag();

		void Render(unsigned int VAO) override;
		void Update() override;

		void CalculateDock();


		ElementDock getDock();
		float getDockRatio();

		void setDock(ElementDock dock);
		void setDockRatio(float ratio);

	protected:
		ElementDock dock;
		float dock_ratio;
	};
}

#endif // DIV_H
