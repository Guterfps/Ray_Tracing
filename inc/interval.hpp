
#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "utils.hpp"

namespace RayTracing {

class Interval {
public:
    static const Interval EMPTY, UNIVERSE;

    Interval();
    Interval(double min, double max);
    Interval(const Interval& a, const Interval& b);

    double GetMin() const;
    double GetMax() const;
    void SetMin(double min);
    void SetMax(double max);
    
    double Size() const;
    bool Contains(double x) const;
    bool Surrounds(double x) const;
    double Clamp(double x) const;
    Interval Expand(double delta) const;

private:
    double m_min;
    double m_max;
};

inline Interval::Interval() : m_min(+INF), m_max(-INF) 
{}

inline Interval::Interval(double min, double max) : m_min(min), m_max(max)
{}

inline Interval::Interval(const Interval& a, const Interval& b) :
m_min(std::min(a.GetMin(), b.GetMin())),
m_max(std::max(a.GetMax(), b.GetMax())) 
{}

inline double Interval::GetMin() const {
    return m_min;
}

inline double Interval::GetMax() const {
    return m_max;
}

inline void Interval::SetMin(double min) {
    m_min = min;
}

inline void Interval::SetMax(double max) {
    m_max = max;
}

inline double Interval::Size() const {
    return (m_max - m_min);
}

inline bool Interval::Contains(double x) const {
    return ((x >= m_min) && (x <= m_max));
}

inline bool Interval::Surrounds(double x) const {
    return ((x > m_min) && (x < m_max));
}

inline double Interval::Clamp(double x) const {
    return ((x < m_min) ? m_min : ((x > m_max) ? m_max : x));
}

inline Interval Interval::Expand(double delta) const {
    double padding = delta / 2;

    return Interval(m_min - padding, m_max + padding);
}

}


#endif // INTERVAL_HPP