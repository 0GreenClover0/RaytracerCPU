#pragma once

#include "AK/Vec3.h"

struct Ray {
    Ray() = default;

    Ray(Vec3 const& origin, Vec3 const& direction) : m_origin(origin), m_direction(direction)
    {
    }

    Vec3 origin() const
    {
        return m_origin;
    }

    Vec3 direction() const
    {
        return m_direction;
    }

    Vec3 at(float time) const
    {
        return m_origin + time * m_direction;
    }

private:
    Vec3 m_origin;
    Vec3 m_direction;
};
