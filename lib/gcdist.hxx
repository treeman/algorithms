#pragma once

#include "math.hxx"

// Input latitude, longitude of 2 points and a radius.
double gc_dist(double lat1, double long1, double lat2, double long2, double r) {
    lat1 = deg2rad(lat1), long1 = deg2rad(long1);
    lat2 = deg2rad(lat2), long2 = deg2rad(long2);
    return r * acos(cos(lat1) * cos(lat2) * cos(long1 - long2)
            + sin(lat1) * sin(lat2));
}

