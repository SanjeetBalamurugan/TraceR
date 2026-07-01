#include "OpenGLWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Utils/vec3.h"
#include "HardwareRT/RayTracer.h"

static void GLFWFrameSizeCallback(GLFWwindow *window, int width, int height)
{
    OpenGLWindow *OGLWindow = (OpenGLWindow *)glfwGetWindowUserPointer(window);
    OGLWindow->SetSpec(width, height);
    glViewport(0, 0, width, height);
}



void OpenGLWindow::OnInit()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(m_width, m_height, "OPENGL", nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(m_Window, GLFWFrameSizeCallback);
    glfwSetWindowUserPointer(m_Window, this);
    glViewport(0, 0, m_width, m_height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_FrameBufferData.resize(m_width * m_height * 3);

    glGenTextures(1, &m_FrameBufferTextureID);
    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    const char* vertSource = R"glsl(
        #version 330 core
        out vec2 TexCoords;
        void main() {
            float x = -1.0 + float((gl_VertexID & 1) << 2);
            float y = -1.0 + float((gl_VertexID & 2) << 1);
            TexCoords = vec2((x + 1.0) * 0.5, (y + 1.0) * 0.5);
            gl_Position = vec4(x, y, 0.0, 1.0);
        }
    )glsl";

    const char* fragSource = R"glsl(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoords;
        uniform sampler2D screenTexture;
        void main() {
            FragColor = texture(screenTexture, vec2(TexCoords.x, 1.0 - TexCoords.y));
        }
    )glsl";

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertShader);
    glAttachShader(m_ShaderProgram, fragShader);
    glLinkProgram(m_ShaderProgram);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glGenVertexArrays(1, &m_DummyVAO);

    RayTracer::getInstance().SetupWorld();
}

void OpenGLWindow::OnUpdate()
{
    size_t required_size = static_cast<size_t>(m_width) * m_height * 3;
    if (m_FrameBufferData.size() != required_size) {
        m_FrameBufferData.resize(required_size);
        glBindTexture(GL_TEXTURE_2D, m_FrameBufferTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    // Raytracer (Hardware)
    RayTracer::getInstance().Draw(m_FrameBufferData, m_width, m_height);

    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTextureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_FrameBufferData.data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_DummyVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTextureID);
    glUniform1i(glGetUniformLocation(m_ShaderProgram, "screenTexture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGLWindow::OnDestroy()
{
    glDeleteProgram(m_ShaderProgram);
    glDeleteVertexArrays(1, &m_DummyVAO);
    glDeleteTextures(1, &m_FrameBufferTextureID);
}

std::vector<unsigned char> OpenGLWindow::GetFrameBufferData() const
{
    return m_FrameBufferData;
}

