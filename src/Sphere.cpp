#include "Sphere.h"

bool Sphere::hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const
{
    Vec3 origin_center = m_center - ray.origin();
    float a = ray.direction().length_squared();
    float h = Vec3::dot(ray.direction(), origin_center);
    float c = origin_center.length_squared() - m_radius * m_radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0.0f)
    {
        return false;
    }

    float sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrtd) / a;

        if (!ray_t.surrounds(root))
        {
            return false;
        }
    }

    hit_record.t = root;
    hit_record.point = ray.at(hit_record.t);
    Vec3 outward_normal = (hit_record.point - m_center) / m_radius;
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = m_material;
    return true;
}
