#pragma once

#include <random>

class Vec3 {
public:
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator-() const;

    float length() const;

    float length_squared() const;

    static float dot(const Vec3& lhs, const Vec3& rhs);

    static Vec3 cross(const Vec3& lhs, const Vec3& rhs);

    Vec3 normalize() const;

    static Vec3 random(float min = 0.0f, float max = 1.0f);

    bool near_zero() const;

    static Vec3 random_in_unit_sphere();

    static Vec3 random_unit_vector();

    static Vec3 random_on_hemisphere(const Vec3& normal);

    static Vec3 reflect(const Vec3& v, const Vec3& n);

    static Vec3 refract(const Vec3& uv, const Vec3& n, float etai_over_etat);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

    Vec3& operator+=(const Vec3& rhs);

    Vec3& operator-=(const Vec3& rhs);

    Vec3& operator*=(float scalar);

    Vec3& operator/=(float scalar);

    [[nodiscard]] std::string to_string() const;
};

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline Vec3 operator*(const Vec3& v, float scalar)
{
    return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline Vec3 operator*(float scalar, const Vec3& v)
{
    return v * scalar;
}

inline Vec3 operator/(const Vec3& v, float scalar)
{
    return v * (1.0f / scalar);
}

void write_color(std::ofstream& output, const Vec3& color);
