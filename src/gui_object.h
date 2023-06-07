#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <string>
#include <glm/glm.hpp>

#include "any_map.h"

enum class ObjectDock
{
	none,
	top,
	left,
	right,
	bottom,
	center
};

class GUIObject
{
public:
	GUIObject();
	~GUIObject();

	void useDock();
	void calculatePos();

	void setParent(GUIObject *parent);
	void setDock(ObjectDock dock);
	void setPadding(glm::vec4 padding);
	void setMargin(glm::vec4 margin);

	GUIObject* getParent();
	ObjectDock getDock();
	glm::vec4 getPadding();
	glm::vec2 getMargin();

private:
	GUIObject* parent;
	ObjectDock dock;

	glm::vec4 padding;
	glm::vec2 margin;

protected:
	AnyMap<std::string> properties;
};

#endif // UIOBJECT_H