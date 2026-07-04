#pragma once
#include "Utils/vec3.h"
#include "Utils/Intervals.h"
#include "Ray.h"

struct HitData
{
    vec3 hitPoint;
    vec3 normal;
    double t;

    bool front_face;

    void set_face_normal(const Ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hitable
{
public:
    virtual ~Hitable() = default;
    virtual bool hit(const Ray &r, Interval r_interval, HitData &rec) const = 0;
};

class Sphere : public Hitable
{
public:
    Sphere(const vec3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const Ray &r, Interval r_interval, HitData &rec) const override
    {
        double ray_tmin = r_interval.min;
        double ray_tmax = r_interval.max;
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        // Suitable t Value within t_min, t_max
        auto sqrtd = std::sqrt(discriminant);
        auto root = (h - sqrtd) / a;
        if (!r_interval.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!r_interval.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.hitPoint = r.at(rec.t);
        rec.normal = (rec.hitPoint - center) / radius;

        vec3 outward_normal = (rec.hitPoint - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    vec3 center;
    double radius;
};