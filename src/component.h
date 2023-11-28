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
#include "hitypes.h"

namespace higui
{
	class Event 
	{
	public:
		Event(GLFWwindow* win);

		using EventFunction = std::function<void()>;

		// Mouse
		class Mouse
		{
		public:
			bool hovered(EventFunction = nullptr) noexcept;
			glm::ivec2 moved(EventFunction = nullptr) noexcept;
			glm::ivec2 scrolledVert(EventFunction = nullptr) noexcept;
			glm::ivec2 dragged(int& button, EventFunction = nullptr) noexcept;

			glm::ivec2 pressed(int& button, EventFunction = nullptr) noexcept;
			glm::ivec2 released(int& button, EventFunction = nullptr) noexcept;
		}; // class Mouse

		// Keyboard
		class Keyboard
		{
		public:
			void pressed(int& key, int& scancode, int& action, int& mods) noexcept;
			void released(int& key, int& scancode, int& action, int& mods) noexcept;
			EventFunction pressedFunc{ nullptr };
			EventFunction releasedFunc{ nullptr };
		}; // class Keyboard

		// Window
		class Window
		{
		public:
			Window(GLFWwindow* window) : window_ptr(window) {}
			glm::ivec2 Resized() noexcept;
			bool Minimized() noexcept;
			bool Focused() noexcept;
			void Closed() noexcept;

			GLFWwindow* window_ptr;
		}; // class Window

		Mouse mouse;
		Keyboard key;
		Window window;

	private:
		internal::GUIObject* obj;

		static void MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
		static void FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
		static void KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);

		void MouseButtonCallback(int button, int action, int mods);
		void CursorPosCallback(double xpos, double ypos);
		void ScrollCallback(double xoffset, double yoffset);
		void FramebufferSizeCallback(int width, int height);
		void KeyCallback(int key, int scancode, int action, int mods);
	};

	class GUIController {
	private:
		internal::GUIObject* obj;

	public:
		GUIController(internal::GUIObject* obj) : obj(obj) {}

		virtual void Handle(Event event);
		virtual void Update();
		virtual void Render();

	protected:
	};
}