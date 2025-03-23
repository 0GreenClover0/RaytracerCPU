#pragma once

#include "AABB.h"
#include "Hittable.h"

class BVHNode;

class Raytracer
{
public:
    void run();

    std::shared_ptr<BVHNode> m_root = {};

private:
    AABB m_bbox = {Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)};
};
