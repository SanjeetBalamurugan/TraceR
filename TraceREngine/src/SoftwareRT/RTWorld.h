#pragma once
#include "Hitable.h"
#include "Utils/Intervals.h"

#include <vector>
#include <memory>

#define RESERVE_START 100 // hmm, need to give a good name for this

class RTWorld : public Hitable
{
private:
    std::vector<std::shared_ptr<Hitable>> m_WorldObjects;
public:
    RTWorld() { m_WorldObjects.reserve(RESERVE_START); }
    RTWorld(std::shared_ptr<Hitable> obj)
    {
        m_WorldObjects.reserve(RESERVE_START);
        m_WorldObjects.push_back(obj);
    }

    void Clear() { m_WorldObjects.clear(); }
    void Add(std::shared_ptr<Hitable> obj)
    {
        m_WorldObjects.push_back(obj);
    }

    bool hit(const Ray& r, Interval r_interval, HitData& rec) const override {
        double ray_tmin = r_interval.min;

        HitData temp_rec;
        bool hit_anything = false;
        auto closest_so_far = r_interval.max;

        for (const auto& object : m_WorldObjects) {
            if (object->hit(r, Interval(ray_tmin, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};