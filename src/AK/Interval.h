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

    static Interval empty()
    {
        return {FLT_MAX, FLT_MIN};
    }

    static Interval whole()
    {
        return {FLT_MIN, FLT_MAX};
    }
};
