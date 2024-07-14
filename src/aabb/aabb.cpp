
#include "aabb.hpp"

namespace RayTracing {

const AABB AABB::EMPTY{Interval::EMPTY, Interval::EMPTY, Interval::EMPTY};
const AABB AABB::UNIVERSE{Interval::UNIVERSE, Interval::UNIVERSE, Interval::UNIVERSE};


}