#include "DOM.h"

namespace higui
{
	DOM::~DOM()
	{
	}

	void DOM::Render(GLFWwindow* window)
	{
		int win_width, win_height;
		glfwGetWindowSize(window, &win_width, &win_height);
		
		markup.central_object->Render();
	}
}