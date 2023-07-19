#ifndef DOM_H
#define DOM_H

#include "markup_parser.h"
#include "shader.h"

namespace higui
{

	class DOM
	{
	public:
		DOM(std::string markup_file, std::string style_file);

		void Render();
		void HandleEvents();
		void Delete();

		MarkupParser markup;
		ShaderManager shaders;

		private:
			static float vertices[12];
			static unsigned int indices[6];
			unsigned int VBO, VAO, EBO;
	};
}

#endif // DOM_H
