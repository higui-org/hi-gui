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

		void Render(unsigned int VAO) override;
		void Update() override;

		void CalculateDock();

		AttributeDock getDock();
		DockPosition getDockPosition();
		float getDockRatio();

		void setDock(AttributeDock dock);
		void setDockPosition(DockPosition dock_pos);
		void setDockRatio(float ratio);

	protected:
		AttributeDock dock;
	};
}

#endif // DIV_H
