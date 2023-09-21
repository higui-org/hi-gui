#ifndef DIV_H
#define DIV_H

#include "object.h"

namespace higui 
{
	class DivTag : public GUIObjectImpl<DivTag>
	{
	public:
		DivTag();
		~DivTag();

		void Render(unsigned int VAO);
		void Update() override;

		void CalculateDock();

		Dock getDock();
		DockPosition getDockPosition();
		float getDockRatio();

		void setDock(Dock dock);
		void setDockPosition(DockPosition dock_pos);
		void setDockRatio(float ratio);

	protected:
		Dock dock;
	};
}

#endif // DIV_H
