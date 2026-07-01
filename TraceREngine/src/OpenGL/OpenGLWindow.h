#pragma once

#include "Core.h"
#include "Core/IWindow.h"

#include <GLFW/glfw3.h>

class OpenGLWindow : public IWindow
{
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnDestroy() override;

    GLFWwindow* GetWindow() override { return m_Window; }
    WindowSpec GetSpec() override 
    {
        WindowSpec spec;
        spec.width = m_width;
        spec.height = m_height;

        return spec;
    }

    void SetSpec(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    std::vector<unsigned char> GetFrameBufferData() const override;

private:
    GLFWwindow *m_Window;
    int m_width = 500;
    int m_height = 500;
    float aspect_ratio = 1;

    unsigned int m_DummyVAO;
    unsigned int m_ShaderProgram;

    GLuint m_FrameBufferTextureID;
    std::vector<unsigned char> m_FrameBufferData;
};