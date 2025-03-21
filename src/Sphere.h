#pragma once

#include "AK/Interval.h"
#include "AK/Vec3.h"
#include "Hittable.h"
#include "Ray.h"

class Sphere final : public Hittable
{
public:
    Sphere(Vec3 const& center, float radius, std::shared_ptr<Material> const& material) : m_center(center), m_radius(radius), m_material(material)
    {
    }

    virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

private:
    Vec3 m_center;
    float m_radius;
    std::shared_ptr<Material> m_material;
};
