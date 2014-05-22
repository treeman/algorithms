#pragma once

const double PI = acos(-1.0); // alternative (2.0 * acos(0.0))
const double EPS = 1e-9;

double rad2deg(double rad) {
    return rad * 180 / PI;
}

double deg2rad(double degree) {
    return degree * PI / 180;
}
