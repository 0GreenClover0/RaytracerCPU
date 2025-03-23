#pragma once

#include "AK/Interval.h"
#include "Ray.h"

struct AABB
{
    Interval x;
    Interval y;
    Interval z;

    AABB() = default;

    AABB(const AABB& box0, const AABB& box1);

    AABB(const Interval& x, const Interval& y, const Interval& z);

    AABB(const Vec3& a, const Vec3& b);

    [[nodiscard]] const Interval& axis_interval(const int n) const;

    [[nodiscard]] bool hit(const Ray& ray, Interval ray_t) const;

    [[nodiscard]] int longest_axis() const;

    static const AABB empty;
    static const AABB whole;
};
