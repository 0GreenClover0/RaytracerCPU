#pragma once

#include <corecrt_math_defines.h>
#include <math.h>
#include <stdint.h>
#include <random>

static float radians(const float degrees)
{
    return degrees * (M_PI / 180.0f);
}

static float random_float()
{
    return static_cast<float>(rand()) / static_cast <float>(RAND_MAX);
}

static float random_float(float min, float max)
{
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

static float clamp(const float value, const float min, const float max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}
