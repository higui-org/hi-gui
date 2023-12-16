#include "component.h"

namespace hi
{
	namespace internal
	{
		inline Behavior::Mod operator|(Behavior::Mod a, Behavior::Mod b) {
			return static_cast<Behavior::Mod>(static_cast<int>(a) | static_cast<int>(b));
		}

		EventHandler::EventHandler(GLFWwindow* window) : win(window)
		{
			glfwSetWindowUserPointer(window, this);

			glfwSetKeyCallback(window, EventHandler::KeyCallbackWrapper);
			glfwSetScrollCallback(window, EventHandler::ScrollCallbackWrapper);
			glfwSetCursorPosCallback(window, EventHandler::CursorPosCallbackWrapper);
			glfwSetMouseButtonCallback(window, EventHandler::MouseButtonCallbackWrapper);
			glfwSetFramebufferSizeCallback(window, EventHandler::FramebufferSizeCallbackWrapper);
		}

		void EventHandler::MouseButtonCallback(int button, int action, int mods)
		{

		}

		void EventHandler::CursorPosCallback(double xpos, double ypos)
		{
			
		}

		void EventHandler::ScrollCallback(double xoffset, double yoffset)
		{

		}

		void EventHandler::FramebufferSizeCallback(int width, int height)
		{

		}

		void EventHandler::KeyCallback(int key, int scancode, int action, int mods)
		{

		}



		void EventHandler::MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (handler)
				handler->MouseButtonCallback(button, action, mods);
		}

		void EventHandler::CursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (handler) 
				handler->CursorPosCallback(xpos, ypos);
		}

		void EventHandler::ScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (handler)
				handler->ScrollCallback(xoffset, yoffset);
		}

		void EventHandler::FramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (handler)
				handler->FramebufferSizeCallback(width, height);
		}

		void EventHandler::KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (handler)
				handler->KeyCallback(key, scancode, action, mods);
		}
	}; // namespace internal

	void Behavior::HandleEvent(Event* e)
	{
		
	}
}