#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef ENGINE_SUPPORT_VULKAN
    #include <vulkan/vulkan.h>
#endif

#ifdef ENGINE_SUPPORT_D3D12
    #include <d3d12.h>
    #include <dxgi1_6.h>
    #include <d3dcompiler.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"