
#include <algorithm>

#include "bvh.hpp"

namespace RayTracing {

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects,
                size_t start, size_t end) {

    m_bbox = AABB::EMPTY;

    for (size_t object_index = start; object_index < end; ++object_index) {
        m_bbox = AABB(m_bbox, objects[object_index]->BoundingBox());
    }

    auto axis = m_bbox.LongestAxis();

    auto comparator = ChoseCompare(axis);

    size_t object_span = end - start;

    if (object_span == 1) {
        m_left = m_right = objects[start];
    }
    else if (object_span == 2) {
        m_left = objects[start];
        m_right = objects[start + 1];
    }
    else {
        std::sort(objects.begin() + start, 
                objects.begin() + end, 
                comparator);

        size_t mid = start + object_span / 2;
        m_left = std::make_shared<BVHNode>(objects, start, mid);
        m_right = std::make_shared<BVHNode>(objects, mid, end);
    }
}

bool BVHNode::Hit(const Ray& ray, 
            const Interval& ray_t, 
            HitRecord& rec) const {
    if (!m_bbox.Hit(ray, ray_t)) {
        return false;
    }

    bool hit_left = m_left->Hit(ray, ray_t, rec);
    bool hit_right = m_right->Hit(ray, 
                    Interval(ray_t.GetMin(), hit_left ? rec.t : ray_t.GetMax()),
                    rec);

    return (hit_left || hit_right);
}

inline bool BVHNode::BoxCompare(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b, 
                        AABB::Axis axis_index) {
    Interval a_axis = a->BoundingBox().AxisInterval(axis_index);
    Interval b_axis = b->BoundingBox().AxisInterval(axis_index);

    return (a_axis.GetMin() < b_axis.GetMin());
}

bool BVHNode::BoxCompareX(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, AABB::Axis::X);
}

bool BVHNode::BoxCompareY(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, AABB::Axis::Y);
}

bool BVHNode::BoxCompareZ(const std::shared_ptr<Hittable>& a,
                        const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, AABB::Axis::Z);
}

inline std::function<bool(const std::shared_ptr<Hittable>&,
                        const std::shared_ptr<Hittable>&)> 
            BVHNode::ChoseCompare(AABB::Axis axis_index) {
    using Axis = AABB::Axis;
    
    std::function<bool(const std::shared_ptr<Hittable>&,
                    const std::shared_ptr<Hittable>&)> compare;

    switch (axis_index) {
        case Axis::X :
            compare = &BoxCompareX;
            break;
        case Axis::Y :
            compare = &BoxCompareY;
            break;
        case Axis::Z :
            compare = &BoxCompareZ;
            break;
        default:
            break;
    }

    return compare;
}

}