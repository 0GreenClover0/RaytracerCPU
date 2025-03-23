#include "BVHNode.h"

#include "AK/MathUtils.h"

#include <algorithm>

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& hittables, const size_t start, const size_t end)
{
    // Build the bounding box of the span of source hittables.
    m_bbox = AABB::empty;

    for (size_t index = start; index < end; ++index)
    {
        m_bbox = AABB(m_bbox, hittables[index]->bounding_box());
    }

    const int axis = m_bbox.longest_axis();

    const auto comparator = axis == 0 ? box_x_compare : axis == 1 ? box_y_compare : box_z_compare;

    const size_t hittables_span = end - start;

    if (hittables_span == 1)
    {
        m_hittable_left = m_hittable_right = hittables[start];
    }
    else if (hittables_span == 2)
    {
        m_hittable_left = hittables[start];
        m_hittable_right = hittables[start + 1];
    }
    else
    {
        std::sort(hittables.begin() + static_cast<long long>(start), hittables.begin() + static_cast<long long>(end), comparator);

        const unsigned long mid = start + hittables_span / 2;

        m_left = std::make_shared<BVHNode>(hittables, start, mid);
        m_right = std::make_shared<BVHNode>(hittables, mid, end);
    }
}

bool BVHNode::hit(const Ray& ray, const Interval ray_t, HitRecord& hit_record) const
{
    if (!m_bbox.hit(ray, ray_t))
    {
        return false;
    }

    // Only check for m_left, because m_right should be null when the m_left is, and not null when the m_left is not.
    if (m_left == nullptr)
    {
        const bool hit_left = m_hittable_left->hit(ray, ray_t, hit_record);
        const bool hit_right = m_hittable_right->hit(ray, Interval(ray_t.min, hit_left ? hit_record.t : ray_t.max), hit_record);
        return hit_left || hit_right;
    }

    const bool hit_left = m_left->hit(ray, ray_t, hit_record);
    const bool hit_right = m_right->hit(ray, Interval(ray_t.min, hit_left ? hit_record.t : ray_t.max), hit_record);

    return hit_left || hit_right;
}

AABB BVHNode::bounding_box() const
{
    return m_bbox;
}

bool BVHNode::box_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, const int axis_index)
{
    return a->bounding_box().axis_interval(axis_index).min < b->bounding_box().axis_interval(axis_index).min;
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
{
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
{
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
{
    return box_compare(a, b, 2);
}
