#pragma once

#include <cfloat>

class Interval
{
public:
    float min;
    float max;

    Interval()
    {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    Interval(const float min, const float max) : min(min), max(max)
    {
    }

    Interval(const Interval& a, const Interval& b)
    {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    float size() const
    {
        return max - min;
    }

    bool contains(float x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const
    {
        return min < x && x < max;
    }

    Interval expand(float const delta) const
    {
        float const padding = delta / 2.0f;

        return Interval(min - padding, max + padding);
    }

    static Interval const empty;
    static Interval const whole;
};
