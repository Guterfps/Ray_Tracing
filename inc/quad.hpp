
#ifndef QUAD_HPP
#define QUAD_HPP

#include "hittable.hpp"

namespace RayTracing {

class Quad : public Hittable {
public:
    Quad(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat);

    virtual void SetBoundingBox();

    AABB BoundingBox() const override;
    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    
    virtual bool IsInterior(double a, double b) const; 

private:
    AABB m_bbox;
    Point3 m_Q;
    Vec3 m_u;
    Vec3 m_v;
    Vec3 m_w;
    Vec3 m_normal;
    double m_D;
    std::shared_ptr<Material> m_mat;

    static Vec3 CalcPlaneNormal(const Vec3& u, const Vec3& v);
    static double CalcPlaneD(const Vec3& normal, const Point3& p);
    static Vec3 CalcW(const Vec3& u, const Vec3& v);

};

inline Quad::Quad(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat) :
m_Q(Q), m_u(u), m_v(v), m_w(CalcW(u, v)),
m_normal(CalcPlaneNormal(m_u, m_v)), m_D(CalcPlaneD(m_normal, m_Q)), 
m_mat(mat) 
{
    SetBoundingBox();
}

inline void Quad::SetBoundingBox() {
    AABB bbox_diagonal1(m_Q, m_Q + m_u + m_v);
    AABB bbox_diagonal2(m_Q + m_u, m_Q + m_v);

    m_bbox = AABB(bbox_diagonal1, bbox_diagonal2);
}

inline AABB Quad::BoundingBox() const {
    return m_bbox;
}

inline bool Quad::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    double denom = Dot(m_normal, ray.GetDirection());

    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    double t = (m_D - Dot(m_normal, ray.GetOrigin())) / denom;
    if (!ray_t.Contains(t)) {
        return false;
    }

    // Determine the hit point lies within the planar shape using its plane coordinates.
    Point3 intersection = ray.At(t);
    Vec3 planar_hitpt_vector = intersection - m_Q;
    double alpha = Dot(m_w, Cross(planar_hitpt_vector, m_v));
    double beta = Dot(m_w, Cross(m_u, planar_hitpt_vector));

    if (!IsInterior(alpha, beta)) {
        return false;
    }

    rec.t = t;
    rec.point = intersection;
    rec.mat = m_mat;
    rec.SetFaceNormal(ray, m_normal);
    rec.u = alpha;
    rec.v = beta;

    return true;
}

inline bool Quad::IsInterior(double a, double b) const {
    Interval unit_interval(0.0, 1.0);

    return (unit_interval.Contains(a) && 
            unit_interval.Contains(b));
}

inline Vec3 Quad::CalcPlaneNormal(const Vec3& u, const Vec3& v) {
    return (UnitVector(Cross(u, v)));
}

inline double Quad::CalcPlaneD(const Vec3& normal, const Point3& p) {
    return (Dot(normal, p));
}

inline Vec3 Quad::CalcW(const Vec3& u, const Vec3& v) {
    Vec3 n = Cross(u, v);

    return (n / Dot(n ,n)); 
}

}

#endif // QUAD_HPP