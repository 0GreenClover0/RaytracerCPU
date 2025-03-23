#pragma once

#include "Hittable.h"

class BVHNode
{
public:
    BVHNode(std::vector<std::shared_ptr<Hittable>>& hittables, const size_t start, const size_t end);

    [[nodiscard]] bool hit(const Ray& ray, const Interval ray_t, HitRecord& hit_record) const;

    [[nodiscard]] AABB bounding_box() const;

private:
    static bool box_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, const int axis_index);
    static bool box_x_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
    static bool box_y_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
    static bool box_z_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);

    std::shared_ptr<Hittable> m_hittable_left = {};
    std::shared_ptr<Hittable> m_hittable_right = {};
    std::shared_ptr<BVHNode> m_left = {};
    std::shared_ptr<BVHNode> m_right = {};
    AABB m_bbox = {};
};
