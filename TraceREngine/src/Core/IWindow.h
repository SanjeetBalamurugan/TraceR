#pragma once
#include "Core.h"

#include "GLFW/glfw3.h"
#include <vector>

struct WindowSpec
{
    int width;
    int height;
};


class TRACER_API IWindow
{
public:
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnDestroy() = 0;

    virtual std::vector<unsigned char> GetFrameBufferData() const = 0;

    virtual GLFWwindow* GetWindow() = 0;
    virtual WindowSpec GetSpec() = 0;
};
