#pragma once

#include "AK/Interval.h"
#include "AK/MathUtils.h"
#include "AK/Vec3.h"
#include "Hittable.h"
#include "Ray.h"

#include <fstream>

class Camera
{
public:
    float vertical_fov = 90.0f;
    Vec3 look_from;
    Vec3 look_at;
    Vec3 v_up;

    Camera(const float aspect_ratio, const int image_width, const int samples_per_pixel, const int max_depth)
        : look_from(Vec3()), look_at(Vec3(0.0f, 0.0f, -1.0f)), v_up(Vec3(0.0f, 1.0f, 0.0f)), m_aspect_ratio(aspect_ratio),
          m_image_width(image_width), m_samples_per_pixel(samples_per_pixel), m_max_depth(max_depth)
    {
    }

    void render(const Hittable& world);

private:
    float m_aspect_ratio = 1.0f;
    int m_image_width = 100;
    int m_samples_per_pixel = 50;
    int m_max_depth = 50;

    int m_image_height = 0;
    float pixel_samples_scale = 1.0f;
    Vec3 center = {};
    Vec3 pixel00_location = {};
    Vec3 pixel_delta_u = {};
    Vec3 pixel_delta_v = {};
    Vec3 u = {}, v = {}, w = {};

    void initialize();
    Vec3 ray_color(const Ray& ray, int depth, const Hittable& world) const;
    Ray get_ray(int i, int k) const;
    Vec3 sample_square() const;
};

inline Vec3 Camera::ray_color(const Ray& ray, int depth, const Hittable& world) const
{
    if (depth <= 0)
    {
        return {};
    }

    HitRecord hit_record;
    if (world.hit(ray, Interval(0.001f, std::numeric_limits<float>::max()), hit_record))
    {
        Ray scattered;
        Vec3 attenuation;
        if (hit_record.material->scatter(ray, hit_record, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, depth - 1, world);
        }

        return {};
    }

    Vec3 unit_direction = ray.direction().normalize();
    float a = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - a) * Vec3(1.0f, 1.0f, 1.0f) + a * Vec3(0.5f, 0.7f, 1.0f);
}

inline Ray Camera::get_ray(int i, int k) const
{
    Vec3 offset = sample_square();
    Vec3 pixel_sample = pixel00_location + (i + offset.x) * pixel_delta_u + (k + offset.y) * pixel_delta_v;
    Vec3 ray_origin = center;
    Vec3 ray_direction = pixel_sample - ray_origin;
    return Ray(ray_origin, ray_direction);
}

inline Vec3 Camera::sample_square() const
{
    return Vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f);
}

inline void Camera::render(const Hittable& world)
{
    initialize();

    std::ofstream output("output.ppm");
    output << "P3\n" << m_image_width << " " << m_image_height << "\n255\n";

    for (int k = 0; k < m_image_height; ++k)
    {
        for (int i = 0; i < m_image_width; ++i)
        {
            Vec3 pixel_color;
            for (int sample = 0; sample < m_samples_per_pixel; ++sample)
            {
                Ray ray = get_ray(i, k);
                pixel_color += ray_color(ray, m_max_depth, world);
            }

            write_color(output, pixel_color * pixel_samples_scale);
        }
    }
}
