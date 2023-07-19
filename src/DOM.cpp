#include "DOM.h"

namespace higui
{	
	DOM::DOM(const std::string& markup_file, const std::string& style_file)
		: markup(markup_file)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void DOM::Render()
	{	
		markup.central_object->Render(VAO);
	}

	void DOM::Delete() 
	{
		shaders.Delete();
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	float DOM::vertices[12] = {
				-1.0f, 1.0f, 0.0f,   // upper left angle
				1.0f, 1.0f, 0.0f,  // upper right angle
				1.0f, -1.0f, 0.0f, // lower right angle
				-1.0f, -1.0f, 0.0f   // lower left angle
	};

	unsigned int DOM::indices[6] = {
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
	};
}