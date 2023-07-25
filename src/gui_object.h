#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "custom_any.h"
#include "shader.h"

namespace higui
{
	class DOM;

	enum class ElementDock
	{
		none,
		top,
		bottom,
		left,
		right
	};

	class GUIObject
	{
	public:
		GUIObject();
		virtual ~GUIObject();
		
		virtual void Render(unsigned int VAO);
		virtual void Update();

		void set(std::string attribute, std::string value);

		template <typename T> 
		T get(std::string attribute) {
			if (properties.find(attribute) == properties.end())
				throw std::runtime_error("Cannot find the attribute in GUI object");
			return FromString<T>(properties[attribute]);
		}
		std::string get(std::string attribute);

		GUIObject* getParent();
		glm::mat4 getModel();

		// events
		bool OnCursorPos(double xpos, double ypos);
		bool OnMouseClick(int button, double xpos, double ypos);

	protected:
		void AddChild(GUIObject* obj);

		GUIObject* parent;
		std::vector<GUIObject*> children;

		glm::mat4 model;

	private:
		std::unordered_map<std::string, std::string> properties;

		friend class MarkupParser;
	};
}

#endif // GUI_OBJECT_H
