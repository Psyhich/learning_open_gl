#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H

#include <memory>
#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

inline const char *TranslateGLError(int errCode)
{
	switch (errCode)
	{
		case GL_INVALID_ENUM:                  return "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 return "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             return "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: return "NO_ERROR";
	}
}

#define CHECK_GL_ERRORS()\
if(int err = glGetError(); err != GL_NO_ERROR)\
{\
	fmt::print("Error {} on {}\n", TranslateGLError(err), __LINE__);\
}

namespace GLFW
{

	struct GLFWContext
	{
	public:
		bool Init();

		~GLFWContext()
		{
			if(m_initialized)
			{
				glfwTerminate();
			}
		}
	private:
		bool m_initialized{false};
	};

	class Window
	{
		struct GLFWWindowDestroyer
		{
			void operator()(GLFWwindow *window)
			{
				glfwDestroyWindow(window);
			}
		};
		using PGLFWwindow = std::unique_ptr<GLFWwindow, GLFWWindowDestroyer>;

	public:
		bool Init(int windowWidth, int windowHeight, const std::string &windowTitle);

		inline bool ShouldClose() const noexcept
		{
			return glfwWindowShouldClose(m_window.get());
		}

		const GLFWwindow *GetWindow() const noexcept
		{
			return m_window.get();
		}

		GLFWwindow *GetWindow() noexcept
		{
			return m_window.get();
		}
	private:
		static void FramebufferResizeCallback(GLFWwindow *window, int width, int height);

	private:
		PGLFWwindow m_window;
		int m_framebufferWidth;
		int m_framebufferHeight;
	};

	void GLFWErrorCallback(int error, const char* description);
}

#endif // GLFW_WRAPPER_H
