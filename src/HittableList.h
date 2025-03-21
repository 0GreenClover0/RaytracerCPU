#pragma once

#include "AK/Interval.h"
#include "Hittable.h"
#include "Ray.h"

#include <vector>
#include <memory>

class HittableList final : public Hittable
{
public:
    HittableList() = default;
    explicit HittableList(std::shared_ptr<Hittable> const& hittable)
    {
        add(hittable);
    }

    void clear()
    {
        m_hittables.clear();
    }

    void add(std::shared_ptr<Hittable> const& hittable)
    {
        m_hittables.emplace_back(hittable);
    }

    virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override
    {
        HitRecord temp_record;
        bool hit_anything = false;
        float closest_so_far = ray_t.max;

        for (const auto& hittable : m_hittables)
        {
            if (hittable->hit(ray, Interval(ray_t.min, closest_so_far), temp_record))
            {
                hit_anything = true;
                closest_so_far = temp_record.t;
                hit_record = temp_record;
            }
        }

        return hit_anything;
    }

private:
    std::vector<std::shared_ptr<Hittable>> m_hittables = {};
};
