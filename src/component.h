#include <vector>
#include <memory>
#include <bitset>

// glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// higui
#include "attribute.h"

namespace hi
{
	class DOM {
	public: 
		internal::EventHandler event;
	};

	enum class Event
	{
		None,
		MouseEvent,
		KeyboardEvent,
		WindowEvent,
		Last,
	};

	class Behavior {
		void HandleEvent(Event* event);

		friend class DOM;

		template <typename T>
		void setSingleType() {
			static_assert(std::is_base_of<GUIObject, T>::value, "T must be a derived class of GUIObject");
			types.push_back(typeid(T));
		}

		bool hasType(std::type_index type) {
			return std::find(types.begin(), types.end(), type) != types.end();
		}

	public:
		// Registration by types
		template <typename... Types>
		void setTypes() {
			(setSingleType<Types>(), ...);
		}

		// Registration by objects
		template <typename... Objects>
		void setTypes(Objects&&... objects) {
			(setSingleType<decltype(objects)>(), ...);
		}

		std::vector<std::type_index> types;
		AttributeContainer attr;

		virtual void Update() {}
		virtual void Render() {}

		enum class Mod;
		enum class Action;
		enum class Window;

		class Mouse
		{
		public:
			using Position = glm::ivec2;
			enum class Button {
				Left = GLFW_MOUSE_BUTTON_LEFT,
				Right = GLFW_MOUSE_BUTTON_RIGHT,
				Middle = GLFW_MOUSE_BUTTON_MIDDLE,
				Button1 = GLFW_MOUSE_BUTTON_1,
				Button2 = GLFW_MOUSE_BUTTON_2,
				Button3 = GLFW_MOUSE_BUTTON_3,
				Button4 = GLFW_MOUSE_BUTTON_4,
				Button5 = GLFW_MOUSE_BUTTON_5,
				Button6 = GLFW_MOUSE_BUTTON_6,
				Button7 = GLFW_MOUSE_BUTTON_7,
				Button8 = GLFW_MOUSE_BUTTON_8,
				Last = GLFW_MOUSE_BUTTON_LAST
			};

			enum class Scroll {
				None,
				Vertical,
				Horizontal,
			};

			glm::vec2 getScreenPos(GLFWwindow* window) { double x, y; glfwGetCursorPos(window, &x, &y); return { x,y }; }

			Action action;
			Scroll scroll{ Scroll::None };
			float scroll_offset = 0.0f;
			Mod mods{ Mod::None };
		};

		class Keyboard
		{
		public:

		};

		// Virtual methods for events



		// Attributes and methods 


		enum class Mod {
			None = 0,
			Shift = GLFW_MOD_SHIFT,
			Control = GLFW_MOD_CONTROL,
			Alt = GLFW_MOD_ALT,
			Super = GLFW_MOD_SUPER,
			CapsLock = GLFW_MOD_CAPS_LOCK,
			NumLock = GLFW_MOD_NUM_LOCK
		};

		enum class Action {
			Release = GLFW_RELEASE,
			Press = GLFW_PRESS,
			Repeat = GLFW_REPEAT
		};

		// Window
		enum class Window {
			Moved,
			Resized,
			Minimized,
			Maximized,
			Focused,
			LostFocus,
		};
	}; // class hiBehavior









	//
	// internal
	//
	namespace internal
	{
		class EventHandler
		{
			GLFWwindow* win;
			std::vector<std::shared_ptr<Behavior>> behaviors;

			// callbacks (wrappers)

			static void MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
			static void CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
			static void ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
			static void FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
			static void KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);

			// callbacks

			void MouseButtonCallback(int button, int action, int mods);
			void CursorPosCallback(double xpos, double ypos);
			void ScrollCallback(double xoffset, double yoffset);
			void FramebufferSizeCallback(int width, int height);
			void KeyCallback(int key, int scancode, int action, int mods);

			friend class Behavior;

		public:
			EventHandler::EventHandler(GLFWwindow* win);
		};
	}; // namespace internal
}