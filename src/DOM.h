#ifndef Hi_DOM_H
#define Hi_DOM_H

#include <iostream>
#include <chrono>

#include "markup.h"

namespace higui
{
	class DOM
	{
	public:
		DOM(GLFWwindow* window, const std::string& markup_file, const std::string& style_file);

		void Render();
		void Delete();

		Markup markup;
		hiObject central_object;
		Event event;

		unsigned int VBO, VAO, EBO;	
	};
}

#endif // Hi_DOM_H
