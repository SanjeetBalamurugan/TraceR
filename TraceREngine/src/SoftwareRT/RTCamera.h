#pragma once
#include "Core.h"

#include "RTWorld.h"

#include <vector>
#include "Utils/vec3.h"
#include <functional>

#include "Utils/helpers.h"

class TRACER_API RTCamera
{
private:
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

    void UpdateCameraConsts(int width, int height);
    void DrawPixel(std::vector<unsigned char> &buffer, int width, int x, int y, const vec3 &pixel_color);

    int samples_per_pixel = 8;
    double pixel_samples_scale;

public:
    RTCamera(int width, int height)
    {
        UpdateCameraConsts(width, height);
    }

    RTCamera()
    {
        UpdateCameraConsts(500, 500);
    }

    Ray GetRayAT(int i, int j) const;
    vec3 SampleSquare() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
    void Render(const RTWorld &world, int width, int height, std::vector<unsigned char> &buffer, const std::function<vec3(const Ray&)>& finalRayColor);
};

