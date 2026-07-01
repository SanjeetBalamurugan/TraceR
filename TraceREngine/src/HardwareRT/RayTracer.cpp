#include "RayTracer.h"
#include "Utils/vec3.h"

#include "Ray.h"
#include "Hitable.h"
#include "Utils/constants.h"

void write_color_to_buffer(std::vector<unsigned char> &buffer, int width, int x, int y, const vec3 &pixel_color)
{
    int rbyte = int(255.999 * pixel_color.x());
    int gbyte = int(255.999 * pixel_color.y());
    int bbyte = int(255.999 * pixel_color.z());

    int pixel_index = (y * width + x) * 3;

    buffer[pixel_index] = static_cast<unsigned char>(rbyte);
    buffer[pixel_index + 1] = static_cast<unsigned char>(gbyte);
    buffer[pixel_index + 2] = static_cast<unsigned char>(bbyte);
}

vec3 RayTracer::FinalRayColor(Ray r)
{
    HitData rec;
    if (m_World.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + vec3(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*vec3(1.0, 1.0, 1.0) + a*vec3(0.5, 0.7, 1.0);
}

void RayTracer::SetupWorld()
{
    m_World.Add(std::make_shared<Sphere>(vec3(0,0,-1), 0.5));
    m_World.Add(std::make_shared<Sphere>(vec3(0,-100.5,-1), 100));
}

void RayTracer::Draw(std::vector<unsigned char> &buffer, int width, int height)
{
    if (image_width != width || image_height != height)
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

            vec3 pixel_color = FinalRayColor(r);
            write_color_to_buffer(buffer, width, i, j, pixel_color);
        }
    }
}