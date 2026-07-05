#include "RTCamera.h"

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void RTCamera::UpdateCameraConsts(int width, int height)
{
    aspect_ratio = width / height;
    image_width = width;
    image_height = height;
    viewport_width = viewport_height * (double(image_width) / image_height);

    viewport_u = vec3(viewport_width, 0, 0);
    viewport_v = vec3(0, -viewport_height, 0);
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    pixel_samples_scale = 1.0 / samples_per_pixel;
}

void RTCamera::DrawPixel(std::vector<unsigned char> &buffer, int width, int x, int y, const vec3 &pixel_color)
{
    int rbyte = int(255.999 * linear_to_gamma(pixel_color.x()));
    int gbyte = int(255.999 * linear_to_gamma(pixel_color.y()));
    int bbyte = int(255.999 * linear_to_gamma(pixel_color.z()));

    int pixel_index = (y * width + x) * 3;

    buffer[pixel_index] = static_cast<unsigned char>(rbyte);
    buffer[pixel_index + 1] = static_cast<unsigned char>(gbyte);
    buffer[pixel_index + 2] = static_cast<unsigned char>(bbyte);
}

Ray RTCamera::GetRayAT(int i, int j) const
{
    vec3 offset = SampleSquare();
    vec3 pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);
    
    vec3 ray_origin = camera_center;
    vec3 ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

void RTCamera::Render(
    const RTWorld& world,
    int width,
    int height,
    std::vector<unsigned char>& buffer,
    const std::function<vec3(const Ray&)>& finalRayColor)
{
    if (image_width != width || image_height != height)
        UpdateCameraConsts(width, height);

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            vec3 pixel_color(0,0,0);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                Ray ray = GetRayAT(i, j);
                pixel_color += finalRayColor(ray);
            }

            pixel_color /= samples_per_pixel;
            DrawPixel(buffer, width, i, j, pixel_color);
        }
    }
}