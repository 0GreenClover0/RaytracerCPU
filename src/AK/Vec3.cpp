#include "Vec3.h"

#include <cmath>
#include <fstream>
#include <string>

Vec3::Vec3(): x(0.0f), y(0.0f), z(0.0f)
{}

Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z)
{}

Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

float Vec3::length() const
{
    return std::sqrt(length_squared());
}

float Vec3::length_squared() const
{
    return x * x + y * y + z * z;
}

float Vec3::dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 Vec3::cross(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

Vec3 Vec3::normalize() const
{
    return *this / length();
}

Vec3 Vec3::random(float min, float max)
{
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return Vec3(distribution(generator), distribution(generator), distribution(generator));
}

bool Vec3::near_zero() const
{
    float constexpr epsilon = 1e-7f;
    return std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon;
}

Vec3 Vec3::random_in_unit_sphere()
{
    while (true)
    {
        Vec3 p = random(-1.0f, 1.0f);

        if (p.length_squared() < 1.0f)
        {
            return p;
        }
    }
}

Vec3 Vec3::random_unit_vector()
{
    return random_in_unit_sphere().normalize();
}

Vec3 Vec3::random_on_hemisphere(const Vec3& normal)
{
    Vec3 on_unit_sphere = random_unit_vector();
    return dot(on_unit_sphere, normal) > 0.0f ? on_unit_sphere : -on_unit_sphere;
}

Vec3 Vec3::reflect(const Vec3& v, const Vec3& n)
{
    return v - 2.0f * dot(v, n) * n;
}

Vec3 Vec3::refract(const Vec3& uv, const Vec3& n, float etai_over_etat)
{
    float cos_theta = std::fmin(dot(-uv, n), 1.0f);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0f - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

std::string Vec3::to_string() const
{
    return std::to_string(static_cast<int>(255.999f * x)) + " "
        + std::to_string(static_cast<int>(255.999f * y)) + " "
        + std::to_string(static_cast<int>(255.999f * z)) + '\n';
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
    x += rhs.x; y += rhs.y; z += rhs.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& rhs)
{
    x -= rhs.x; y -= rhs.y; z -= rhs.z;
    return *this;
}

Vec3& Vec3::operator*=(float scalar)
{
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
}

Vec3& Vec3::operator/=(float scalar)
{
    return *this *= 1.0f / scalar;
}

void write_color(std::ofstream& output, const Vec3& color)
{
    output << static_cast<int>(255.999f * color.x) << ' '
        << static_cast<int>(255.999f * color.y) << ' '
        << static_cast<int>(255.999f * color.z) << '\n';
}

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
    return os << v.x << ' ' << v.y << ' ' << v.z;
}
