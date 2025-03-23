#pragma once

#include "AABB.h"
#include "AK/Interval.h"
#include "AK/MathUtils.h"
#include "AK/Vec3.h"
#include "Ray.h"

class Material;

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    std::shared_ptr<Material> material;
    float t;
    bool front_face;

    void set_face_normal(const Ray& ray, const Vec3& outward_normal)
    {
        front_face = Vec3::dot(ray.direction(), outward_normal) < 0.0f;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const
    {
        return false;
    }

    [[nodiscard]] AABB bounding_box() const
    {
        return m_bbox;
    }

protected:
    AABB m_bbox;
};

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, Vec3& attenuation, Ray& scattered) const
    {
        return false;
    }
};

class Lambertian final : public Material
{
public:
    Vec3 albedo;

    explicit Lambertian(const Vec3& a) : albedo(a)
    {
    }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, Vec3& attenuation, Ray& scattered) const override
    {
        Vec3 scatter_direction = hit_record.normal + Vec3::random_unit_vector();

        if (scatter_direction.near_zero())
        {
            scatter_direction = hit_record.normal;
        }

        scattered = Ray(hit_record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal final : public Material
{
public:
    Vec3 albedo;
    float fuzz;

    Metal(const Vec3& a, const float f) : albedo(a), fuzz(f < 1.0f ? f : 1.0f)
    {
    }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, Vec3& attenuation, Ray& scattered) const override
    {
        Vec3 reflected = Vec3::reflect(ray_in.direction().normalize(), hit_record.normal);
        reflected = reflected + fuzz * Vec3::random_unit_vector();
        scattered = Ray(hit_record.point, reflected);
        attenuation = albedo;
        return Vec3::dot(scattered.direction(), hit_record.normal) > 0.0f;
    }
};

class Dielectric final : public Material
{
public:
    float refraction_index;

    explicit Dielectric(const float ri) : refraction_index(ri)
    {
    }

    virtual bool scatter(const Ray& ray_in, const HitRecord& hit_record, Vec3& attenuation, Ray& scattered) const override
    {
        attenuation = Vec3(1.0f, 1.0f, 1.0f);
        float ri = hit_record.front_face ? (1.0f / refraction_index) : refraction_index;

        Vec3 unit_direction = ray_in.direction().normalize();
        float cos_theta = std::min(Vec3::dot(-unit_direction, hit_record.normal), 1.0f);
        float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0f;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_float())
        {
            direction = Vec3::reflect(unit_direction, hit_record.normal);
        } else
        {
            direction = Vec3::refract(unit_direction, hit_record.normal, ri);
        }

        scattered = Ray(hit_record.point, direction);
        return true;
    }

private:
    static float reflectance(const float cosine, const float refraction_index)
    {
        float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
    }
};
