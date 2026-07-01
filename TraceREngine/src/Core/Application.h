#pragma once
#include "Core.h"

#include "RendererTypes.h"
#include <memory>
#include <vector>

class IWindow;

class TRACER_API Application
{
private:
    std::unique_ptr<IWindow> m_Window;

public:
    Application(RenderAPI api);
    void Run();

    ~Application();
};
