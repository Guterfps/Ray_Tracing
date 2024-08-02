
#ifndef QUAD_HPP
#define QUAD_HPP

#include "hittable.hpp"
#include "hittable_list.hpp"

namespace RayTracing {

class Quad : public Hittable {
public:
    Quad(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat);
    virtual ~Quad() = default;
    
    virtual void SetBoundingBox();

    AABB BoundingBox() const override;
    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    double PDFValue(const Point3& origin, const Vec3& direction) const override;
    Vec3 Random(const Point3& origin) const override;

    virtual bool IsInterior(double a, double b) const; 

private:
    AABB m_bbox;
    Point3 m_Q;
    Vec3 m_u;
    Vec3 m_v;
    Vec3 m_w;
    Vec3 m_normal;
    double m_D;
    double m_area;
    std::shared_ptr<Material> m_mat;

    static Vec3 CalcPlaneNormal(const Vec3& u, const Vec3& v);
    static double CalcPlaneD(const Vec3& normal, const Point3& p);
    static Vec3 CalcW(const Vec3& u, const Vec3& v);
    static double CalcArea(const Vec3& u, const Vec3& v);

};

inline Quad::Quad(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat) :
m_Q(Q), m_u(u), m_v(v), m_w(CalcW(u, v)),
m_normal(CalcPlaneNormal(m_u, m_v)), 
m_D(CalcPlaneD(m_normal, m_Q)),
m_area(CalcArea(m_u, m_v)), 
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

inline double Quad::PDFValue(const Point3& origin, const Vec3& direction) const {
    HitRecord rec;
    if (!this->Hit(Ray(origin, direction), Interval(0.001, INF), rec)) {
        return 0.0;
    }

    double distance_squared = rec.t * rec.t * direction.LengthSquared();
    double cosine = std::fabs(Dot(direction, rec.normal) / direction.Length());

    return (distance_squared / (cosine * m_area));
}

inline Vec3 Quad::Random(const Point3& origin) const {
    Point3 p = m_Q + (RandomDouble() * m_u) + (RandomDouble() * m_v);

    return (p - origin);
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

inline double Quad::CalcArea(const Vec3& u, const Vec3& v) {
    return (Cross(u, v).Length());
}

// Returns the 3D box(six sides) that contains the two opposite vertices a & b.
inline std::shared_ptr<HittableList> 
Box(const Point3& a, const Point3& b, std::shared_ptr<Material> mat) {
    auto sides = std::make_shared<HittableList>();

    Point3 min(std::fmin(a.GetX(), b.GetX()),
            std::fmin(a.GetY(), b.GetY()),
            std::fmin(a.GetZ(), b.GetZ()));
    Point3 max(std::fmax(a.GetX(), b.GetX()),
            std::fmax(a.GetY(), b.GetY()),
            std::fmax(a.GetZ(), b.GetZ()));

    Vec3 dx(max.GetX() - min.GetX(), 0.0, 0.0);
    Vec3 dy(0.0, max.GetY() - min.GetY(), 0.0);
    Vec3 dz(0.0, 0.0, max.GetZ() - min.GetZ());

    sides->Add(std::make_shared<Quad>(
                Point3(min.GetX(), min.GetY(), max.GetZ()),
                dx, dy, mat)); // front
    sides->Add(std::make_shared<Quad>(
                Point3(max.GetX(), min.GetY(), max.GetZ()),
                -dz, dy, mat)); // right
    sides->Add(std::make_shared<Quad>(
                Point3(max.GetX(), min.GetY(), min.GetZ()),
                -dx, dy, mat)); // back
    sides->Add(std::make_shared<Quad>(
                Point3(min.GetX(), min.GetY(), min.GetZ()),
                dz, dy, mat)); // left
    sides->Add(std::make_shared<Quad>(
                Point3(min.GetX(), max.GetY(), max.GetZ()),
                dx, -dz, mat)); // top
    sides->Add(std::make_shared<Quad>(
                Point3(min.GetX(), min.GetY(), min.GetZ()),
                dx, dz, mat)); // bottom

    return sides;
}

}

#endif // QUAD_HPP