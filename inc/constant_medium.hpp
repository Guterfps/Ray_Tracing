
#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "hittable.hpp"
#include "isotropic.hpp"

namespace RayTracing {

class ConstantMedium : public Hittable {
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, 
                    double density,
                    std::shared_ptr<Texture> tex);
    ConstantMedium(std::shared_ptr<Hittable> boundary, 
                    double density,
                    const Color& albedo);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    std::shared_ptr<Hittable> m_boundary;
    std::shared_ptr<Material> m_phase_function;
    double m_neg_inv_density;

};

inline ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, 
                    double density,
                    std::shared_ptr<Texture> tex) :
m_boundary(boundary), 
m_phase_function(std::make_shared<Isotropic>(tex)),
m_neg_inv_density(-1 / density)
{}

inline ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, 
                    double density,
                    const Color& albedo) :
m_boundary(boundary), 
m_phase_function(std::make_shared<Isotropic>(albedo)),
m_neg_inv_density(-1 / density)
{}

inline bool ConstantMedium::Hit(const Ray& ray,
                        const Interval& ray_t,
                        HitRecord& rec) const {
    constexpr bool enable_debug = false; // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool debugging = enable_debug && (RandomDouble() < 0.00001);

    HitRecord rec1, rec2;

    if (!m_boundary->Hit(ray, Interval::UNIVERSE, rec1) || 
        !m_boundary->Hit(ray, Interval(rec1.t + 0.0001, INF), rec2)) {
        return false;
    }

    if (debugging) {
        std::clog << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';
    }

    if (rec1.t < ray_t.GetMin()) {
        rec1.t = ray_t.GetMin();
    }
    if (rec2.t > ray_t.GetMax()) {
        rec2.t = ray_t.GetMax();
    }

    if (rec1.t >= rec2.t) {
        return false;
    }

    if (rec1.t < 0.0) {
        rec1.t = 0.0;
    }

    double ray_len = ray.GetDirection().Length();
    double distance_inside_boundary = (rec2.t - rec1.t) * ray_len;
    double hit_distance = m_neg_inv_density * std::log(RandomDouble());

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_len;
    rec.point = ray.At(rec.t);

    if (debugging) {
        std::clog << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.point = " << rec.point << '\n';
    }

    rec.normal = Vec3(1.0, 0.0, 0.0); // arbitrary
    rec.front_face = true; // also arbitrary
    rec.mat = m_phase_function;

    return true;
}

inline AABB ConstantMedium::BoundingBox() const {
    return m_boundary->BoundingBox();
}


}

#endif // CONSTANT_MEDIUM_HPP