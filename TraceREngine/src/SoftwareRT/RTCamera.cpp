#include "RTCamera.h"

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
}

void RTCamera::DrawPixel(std::vector<unsigned char> &buffer, int width, int x, int y, const vec3 &pixel_color)
{
    int rbyte = int(255.999 * pixel_color.x());
    int gbyte = int(255.999 * pixel_color.y());
    int bbyte = int(255.999 * pixel_color.z());

    int pixel_index = (y * width + x) * 3;

    buffer[pixel_index] = static_cast<unsigned char>(rbyte);
    buffer[pixel_index + 1] = static_cast<unsigned char>(gbyte);
    buffer[pixel_index + 2] = static_cast<unsigned char>(bbyte);
}

void RTCamera::Render(const RTWorld &world, int width, int height,
     std::vector<unsigned char> &buffer, const std::function<vec3(const Ray&)>& finalRayColor)
{
    if (image_width != width || image_height != height)
        UpdateCameraConsts(width, height);
    
    double max_i = (width > 1) ? double(width - 1) : 1.0;
    double max_j = (height > 1) ? double(height - 1) : 1.0;

    for (int j = 0; j < height; j++)
    {
        double g_ratio = double(j) / max_j;
        for (int i = 0; i < width; i++)
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            vec3 pixel_color = finalRayColor(r);
            DrawPixel(buffer, width, i, j, pixel_color);
        }
    }
}