#pragma once
#include "Core.h"
#include "Utils/vec3.h"

class TRACER_API Ray
{
public:
    Ray() {}
    Ray(const vec3 &origin, const vec3 &dirction)
        : m_Origin(origin), m_Direction(dirction) {}

    const vec3& origin() const  { return m_Origin; }
    const vec3& direction() const { return m_Direction; }

    vec3 at(double t) const {
        return m_Origin + t*m_Direction;
    }

private:
    vec3 m_Origin;
    vec3 m_Direction;
    float t = 1;
};