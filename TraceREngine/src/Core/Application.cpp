#include "Application.h"
#include <iostream>

#include "OpenGL/OpenGLWindow.h"
#include <GLFW/glfw3.h>

#include "IWindow.h"

static void GLFWErrorCallback(int error_code, const char* description)
{
	std::cerr << "[GLFW Error] (" << error_code << "): " << description << std::endl;
}

Application::Application(RenderAPI api)
{
    if (!glfwInit())
	{
		std::cout << "GLFW Init Failed!!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

    std::cout << "GLFW Compile Version: "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;

    glfwSetErrorCallback(GLFWErrorCallback);

    switch (api)
    {
    case OpenGL:
        m_Window = std::make_unique<OpenGLWindow>();
        break;
    
    default:
        break;
    }
}

void Application::Run()
{
    if (m_Window)
       m_Window->OnInit();
    
    while (!glfwWindowShouldClose(m_Window->GetWindow()))
	{
		glfwPollEvents();

        m_Window->OnUpdate();

		glfwSwapBuffers(m_Window->GetWindow());
	}
}

Application::~Application()
{
    m_Window->OnDestroy();
    glfwTerminate();
}
