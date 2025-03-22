#include "Camera.h"

#include <execution>
#include <iostream>
#include <ranges>
#include <vector>

void Camera::initialize()
{
    pixel_samples_scale = 1.0f / static_cast<float>(m_samples_per_pixel);
    center = look_from;

    float focal_length = (look_from - look_at).length();
    float theta = radians(vertical_fov);
    float h = tan(theta / 2.0f);
    float viewport_height = 2.0f * h * focal_length;
    float viewport_width = viewport_height * (static_cast<float>(m_image_width) / static_cast<float>(m_image_height));

    w = (look_from - look_at).normalize();
    u = Vec3::cross(v_up, w).normalize();
    v = Vec3::cross(w, u);

    Vec3 viewport_u = viewport_width * u;
    Vec3 viewport_v = -viewport_height * v;

    pixel_delta_u = viewport_u / static_cast<float>(m_image_width);
    pixel_delta_v = viewport_v / static_cast<float>(m_image_height);

    Vec3 viewport_upper_left = center - (focal_length * w) - viewport_u / 2.0f - viewport_v / 2.0f;
    pixel00_location = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

void Camera::render(const Hittable& world)
{
    initialize();

    std::ofstream output("output.ppm");
    output << "P3\n" << m_image_width << " " << m_image_height << "\n255\n";

    std::vector<std::string> scanlines = {};
    scanlines.resize(m_image_width * m_image_height);

    auto range = std::views::iota(0, m_image_height);

    std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](int const k)
    {
        for (int i = 0; i < m_image_width; ++i)
        {
            Vec3 pixel_color;
            for (int sample = 0; sample < m_samples_per_pixel; ++sample)
            {
                Ray ray = get_ray(i, k);
                pixel_color += ray_color(ray, m_max_depth, world);
            }

            scanlines[k * m_image_width + i] = (pixel_color * pixel_samples_scale).to_string();
        }
    });

    for (auto const& line : scanlines)
    {
        output << line;
    }

    output.close();

    std::cout << "Done\n";
}
