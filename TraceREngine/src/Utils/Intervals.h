#pragma once
#include "constants.h"

#define NOMINMAX
#undef min
#undef max

class Interval {
  public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default Interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const Interval empty, universe;
};