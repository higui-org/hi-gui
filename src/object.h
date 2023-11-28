#ifndef Hi_GUI_OBJECT_H
#define Hi_GUI_OBJECT_H

// higui
#include "attribute.h"
#include "shader.h"
#include "component.h"

namespace higui
{
	using hiObject = std::shared_ptr<internal::GUIObject>;

	namespace internal
	{
		class GUIObject : public std::enable_shared_from_this<GUIObject>
		{
		public:
			GUIObject(GUIObject* parent_ = nullptr) : parent(parent_), model(1.0f) {}

			void Render(unsigned int VAO);
			void Update();

			template <class V>
			std::enable_if_t<std::is_base_of_v<AttributeValueBase, V>, V&>
				operator()(const std::string& key = "")
			{
				return container.value<V>();
			}

			Attribute& operator()(const std::string& key) {
				return container[key];
			}

			void AddAttr(Attribute attr);

			void ResetModel(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);

			// getters
				hiObject getParent() { return parent; }
				glm::mat4 getModel() { return model; }
			
				// normalized
				glm::vec3 Position();
				glm::vec3 Scale();
				glm::quat Rotation();
				geometry3 Geometry();

				// screen
				glm::vec3 ScreenCoords(GLFWwindow* window);
				glm::vec3 ScreenDimensions(GLFWwindow* window);
				geometry3 ScreenGeometry(GLFWwindow* window);

			// events
			virtual bool OnCursorPos(double xpos, double ypos);
			virtual bool OnMouseClick(int button, double xpos, double ypos);

			// output
			friend std::ostream& operator<<(std::ostream& os, const hiObject& obj);

			template <typename T, typename ... TArgs>
			T& addComponent(TArgs&&... mArgs)
			{
				T* c(new T(std::forward<TArgs>(mArgs)...));
				c->

			}

			static std::unordered_map<std::string, std::function<hiObject()>>& registry()
			{
				static std::unordered_map<std::string, std::function<hiObject()>> registry;
				return registry;
			}

			template <typename T> bool hasComponent() const
			{
				return component_bitset[getComponentID<T>];
			}

		protected:
			void AddChild(hiObject obj_ptr) { children.push_back(obj_ptr); }

			hiObject MouseIn(glm::vec2 point, GLFWwindow* win);

			std::vector<hiObject> children;
			hiObject parent;
			glm::mat4 model;

			std::vector<std::unique_ptr<Component>> components;

			// internal
				// friends~
				friend class DOM;
				friend class Markup;

				// basic rectangle
				static float vertices[12];
				static unsigned int indices[6];

				// components
				ComponentBitSet component_bitset;
				ComponentArray component_array;
		};
	} // end of 'internal' namespace
}

#endif // Hi_GUI_OBJECT_H
