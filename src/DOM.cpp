#include "DOM.h"

namespace higui
{
	void DOM::Render(unsigned int VAO)
	{	
		markup.central_object->Render(VAO);
	}

	void DOM::Delete() 
	{
		shaders.Delete();
	}
}