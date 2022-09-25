#include "glfw_wrapper.h"
#include "fmt/core.h"

using namespace GLFW;

bool GLFWContext::Init()
{
	m_initialized = glfwInit(); 
	if (!m_initialized)
	{
		std::cerr << "GLFW initialization failed!" << std::endl;
		return false;
	}

	return true;
}

bool Window::Init(int windowWidth, int windowHeight, const std::string &windowTitle)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window.reset(glfwCreateWindow(640, 480, windowTitle.c_str(), NULL, NULL));
	if(m_window == nullptr)
	{
		std::cerr << "Window creation failed!" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(m_window.get());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	glfwGetFramebufferSize(m_window.get(), &m_framebufferWidth, &m_framebufferHeight);
	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

	glfwSetFramebufferSizeCallback(m_window.get(), FramebufferResizeCallback);

	return true;
}

void GLFW::GLFWErrorCallback(int error, const char* description)
{
	std::cerr << fmt::format("Got error {}: {}\n", error, description);
}

void Window::FramebufferResizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
