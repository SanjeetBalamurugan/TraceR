#include "RayTracer.h"
#include "Utils/vec3.h"

#include "Ray.h"
#include "Hitable.h"
#include "Utils/constants.h"
#include "Utils/Intervals.h"
#include "Utils/helpers.h"

vec3 RayTracer::FinalRayColor(Ray r, const RTWorld& world, int currBounce)
{
    // Exceeded Max bounce
    if (currBounce <= 0)
        return vec3(0,0,0);

    HitData rec;
    if (world.hit(r, Interval(0.001, infinity), rec)) {
        vec3 direction = random_on_hemisphere(rec.normal) * random_unit_vector();
        return 0.1 * FinalRayColor(Ray(rec.hitPoint, direction), world, currBounce - 1);
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
    m_Camera.Render(m_World, width, height, buffer,
        [this](const Ray& ray) {
            return this->FinalRayColor(ray, this->m_World, this->max_bounces);
        }
    );
}