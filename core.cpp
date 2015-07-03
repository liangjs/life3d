#include "core.h"
#include <cmath>

int HashPoint::operator()(const Point &a)const
{
    int ax = a.x ^ (a.x >> 32);
    int ay = a.y ^ (a.y >> 32);
    int az = a.z ^ (a.z >> 32);
    int fx = (ax & 0xffff) ^ (ay >> 16);
    int fy = (ay & 0xffff) ^ (az >> 16);
    int fz = (az & 0xffff) ^ (ax >> 16);
    return (fx << 22) ^ (fy << 11) ^ fz;
}

static const int dir[27][3] =
{
    {0, 0, 0}, {0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, 1, 1}, {0, 1, -1}, {0, -1, 0}, {0, -1, 1}, {0, -1, -1}
    , {1, 0, 0}, {1, 0, 1}, {1, 0, -1}, {1, 1, 0}, {1, 1, 1}, {1, 1, -1}, {1, -1, 0}, {1, -1, 1}, {1, -1, -1}
    , { -1, 0, 0}, { -1, 0, 1}, { -1, 0, -1}, { -1, 1, 0}, { -1, 1, 1}, { -1, 1, -1}, { -1, -1, 0}, { -1, -1, 1}, { -1, -1, -1}
};

void Board::run()
{
    std::unordered_map<Point, int, HashPoint> cnt;
    for (auto i = data->begin(); i != data->end(); ++i)
        for (int j = 1; j < 27; ++j)
            ++cnt[Point(i->x + dir[j][0], i->y + dir[j][1], i->z + dir[j][2])];
    tmp->clear();
    for (auto i = cnt.begin(); i != cnt.end(); ++i)
        if (getStatus(i->second, data->count(i->first)))
            tmp->insert(i->first);
    std::swap(tmp, data);
}

bool Board::getStatus(int cnt, bool old)
{
    if (cnt > 13)
        return false;
    if (cnt > 9)
        return true;
    if (cnt > 6)
        return old;
    return false;
}

DPoint rotate(const DPoint &p, const DPoint &v, double angle)
{
    double x2 = v.x * v.x;
    double y2 = v.y * v.y;
    double z2 = v.z * v.z;
    double d2 = x2 + y2 + z2;
    double d = sqrt(d2);
    double sina = sin(angle);
    double cosa = cos(angle);
    double fx = (x2 + (y2 + z2) * cosa) / d2 * p.x
                + (v.x * v.y * (1 - cosa) / d2 - v.z * sina / d) * p.y
                + (v.x * v.z * (1 - cosa) / d2 + v.y * sina / d) * p.z;
    double fy = (v.y * v.x * (1 - cosa) / d2 + v.z * sina / d) * p.x
                + (y2 + (x2 + z2) * cosa) / d2 * p.y
                + (v.y * v.z * (1 - cosa) / d2 - v.x * sina / d) * p.z;
    double fz = (v.z * v.x * (1 - cosa) / d2 - v.y * sina / d) * p.x
                + (v.z * v.y * (1 - cosa) / d2 + v.x * sina / d) * p.y
                + (z2 + (x2 + y2) * cosa) / d2 * p.z;
    return DPoint(fx, fy, fz);
}

DPoint cross(const DPoint &a, const DPoint &b)
{
    return DPoint(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double dot(const DPoint &a, const DPoint &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double length(const DPoint &a)
{
    return sqrt(dot(a, a));
}

DPoint operator+ (const DPoint &a, const DPoint &b)
{
    return DPoint(a.x + b.x, a.y + b.y, a.z + b.z);
}

DPoint operator- (const DPoint &a, const DPoint &b)
{
    return DPoint(a.x - b.x, a.y - b.y, a.z - b.z);
}
