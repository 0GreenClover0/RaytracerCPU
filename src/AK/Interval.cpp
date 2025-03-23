#include "Interval.h"

#include <limits>

Interval const Interval::empty = Interval(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
Interval const Interval::whole = Interval(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
