
#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "utils.hpp"

namespace RayTracing {

class Interval {
public:
    static const Interval EMPTY, UNIVERSE;

    Interval();
    Interval(double min, double max);

    double GetMin() const;
    double GetMax() const;
    
    double Size() const;
    bool Contains(double x) const;
    bool Surrounds(double x) const;
    double Clamp(double x) const;

private:
    double m_min;
    double m_max;
};

inline Interval::Interval() : m_min(+INF), m_max(-INF) 
{}

inline Interval::Interval(double min, double max) : m_min(min), m_max(max)
{}

inline double Interval::GetMin() const {
    return m_min;
}

inline double Interval::GetMax() const {
    return m_max;
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

}


#endif // INTERVAL_HPP