#pragma once
#include "Core.h"
#include <vector>

#include "Utils/vec3.h"
#include "Ray.h"
#include "RTWorld.h"
#include "RTCamera.h"

class TRACER_API RayTracer
{
private:
    RayTracer() {}
    ~RayTracer() = default;

    static vec3 FinalRayColor(Ray r, const RTWorld& world, int currBounce);
    RTWorld m_World;
    RTCamera m_Camera;

    int max_bounces = 5;
public:
    RayTracer(const RayTracer&) = delete;
    RayTracer& operator=(const RayTracer&) = delete;
    RayTracer(RayTracer&&) = delete;
    RayTracer& operator=(RayTracer&&) = delete;
    static RayTracer& getInstance() {
        static RayTracer instance;
        return instance;
    }

    void SetupWorld();
    void Draw(std::vector<unsigned char> &buffer, int width, int height);
};