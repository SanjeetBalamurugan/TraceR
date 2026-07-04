#pragma once
#include "Core.h"
#include <vector>

#include "Utils/vec3.h"
#include "Ray.h"
#include "RTWorld.h"

class TRACER_API RayTracer
{
private:
    RayTracer() {}
    ~RayTracer() = default;

    int image_width = 500;
    int image_height = 500;
    float aspect_ratio = 1;

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (double(image_width)/image_height);
    vec3 camera_center = vec3(0, 0, 0);

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;
    vec3 viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    vec3 FinalRayColor(Ray r);
    RTWorld m_World;
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