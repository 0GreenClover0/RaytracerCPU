#include "Camera.h"

void Camera::initialize()
{
    m_image_height = static_cast<int>(m_image_width / m_aspect_ratio);
    m_image_height = std::max(m_image_height, 1);

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
