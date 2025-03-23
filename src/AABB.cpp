#include "AABB.h"

#include <algorithm>

AABB const AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
AABB const AABB::whole = AABB(Interval::whole, Interval::whole, Interval::whole);

AABB::AABB(const AABB& box0, const AABB& box1)
{
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

AABB::AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z)
{
}

AABB::AABB(const Vec3& a, const Vec3& b)
{
    x = a.x <= b.x ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = a.y <= b.y ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = a.z <= b.z ? Interval(a.z, b.z) : Interval(b.z, a.z);
}

Interval const& AABB::axis_interval(const int n) const
{
    if (n == 1)
    {
        return y;
    }

    if (n == 2)
    {
        return z;
    }

    return x;
}

bool AABB::hit(const Ray& ray, Interval ray_t) const
{
    const Vec3& ray_origin = ray.origin();
    const Vec3& ray_direction = ray.direction();

    for (int axis = 0; axis < 3; ++axis)
    {
        Interval const& ax = axis_interval(axis);
        float const adinv = 1.0f / ray_direction[axis];

        float const t0 = (ax.min - ray_origin[axis]) * adinv;
        float const t1 = (ax.max - ray_origin[axis]) * adinv;

        if (t0 < t1)
        {
            ray_t.min = std::max(t0, ray_t.min);

            ray_t.max = std::min(t1, ray_t.max);
        }
        else
        {
            ray_t.min = std::max(t1, ray_t.min);

            ray_t.max = std::min(t0, ray_t.max);
        }

        if (ray_t.max <= ray_t.min)
        {
            return false;
        }
    }

    return true;
}


// Returns the index of the longest axis of the bounding box.
int AABB::longest_axis() const
{
    if (x.size() > y.size())
    {
        return x.size() > z.size() ? 0 : 2;
    }

    return y.size() > z.size() ? 1 : 2;
}
