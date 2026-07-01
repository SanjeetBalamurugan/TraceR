#include "TraceREngine.h"

int main(int argc, char const *argv[])
{
    RenderAPI api = RenderAPI::OpenGL;
    Application app(api);

    app.Run();
    return 0;
}
