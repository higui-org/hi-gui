#ifndef DOM_H
#define DOM_H

#include "markup_parser.h"
#include "shader.h"

namespace higui
{
	class DOM
	{
	public:
		DOM(std::string markup_file, std::string style_file)
			: markup(markup_file)
		{}

		void Render(GLFWwindow* window);
		void HandleEvents(GLFWwindow* window);
		void Delete();

		MarkupParser markup;
		ShaderManager shaders;

	};
}

#endif // DOM_H
