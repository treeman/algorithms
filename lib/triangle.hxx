
template<typename T>
double area(Point<T> a, Point<T> b, Point<T> c) {
    return abs(0.5 * ((c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x)));
}

// Exact double area.
template<typename T>
int area2(Point<T> a, Point<T> b, Point<T> c) {
    return abs(((c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x)));
}

template<typename T>
bool inside(Point<T> p, Point<T> a, Point<T> b, Point<T> c) {
    return area(p, a, b) + area(p, b, c) + area(p, a, c) == area(a, b, c);
}


