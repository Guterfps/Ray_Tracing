
#ifndef BVH_HPP
#define BVH_HPP

#include <functional>

#include "hittable.hpp"
#include "hittable_list.hpp"

namespace RayTracing {

class BVHNode : public Hittable {
public:
    explicit BVHNode(HittableList list);
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects,
            size_t start, size_t end);

    bool Hit(const Ray& ray, 
            const Interval& ray_t, 
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    AABB m_bbox;
    std::shared_ptr<Hittable> m_left;
    std::shared_ptr<Hittable> m_right;

    static bool BoxCompare(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b, 
                        AABB::Axis axis_index);
    static bool BoxCompareX(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b);
    static bool BoxCompareY(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b);
    static bool BoxCompareZ(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b);
    static std::function<bool(const std::shared_ptr<Hittable>&,
                        const std::shared_ptr<Hittable>&)> 
            ChoseCompare(AABB::Axis axis_index);
};

inline BVHNode::BVHNode(HittableList list) :
BVHNode(list.GetObjects(), 0, list.GetSize())
{}

inline AABB BVHNode::BoundingBox() const {
    return m_bbox;
}

}

#endif // BVH_HPP