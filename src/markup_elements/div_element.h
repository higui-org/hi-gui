#ifndef DIV_CLASS_H
#define DIV_CLASS_H

#include "gui_object.h"
#include "custom_any.h"

namespace higui 
{
	class DivElement : public GUIObject
	{
	public:
		DivElement();
		~DivElement();

		void Render(unsigned int VAO) override;
		void Update() override;

		void CalculateDock();


		ElementDock getDock();
		float getDockRatio();

		void setDock(ElementDock dock, float ratio = 0.5f);
		void setDockRatio(float ratio);

	protected:
		ElementDock dock;
		float dock_ratio;
	};
}

#endif // DIV_CLASS_H
