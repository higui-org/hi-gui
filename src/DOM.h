#ifndef DOM_H
#define DOM_H

#include "markup_parser.h"

namespace higui
{
	class DOM
	{
	public:
		DOM(const std::string markup_file, const std::string style_file)
			: markup(markup_file)
		{}

		virtual ~DOM();

		void Render(GLFWwindow* window);
		void HandleEvents(GLFWwindow* window);

		MarkupParser markup;
	};
}

#endif // DOM_H
