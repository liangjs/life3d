#include "core.h"

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

bool Board::getStatus(int cnt, int old)
{
    if (cnt > 13)
        return false;
    if (cnt > 9)
        return true;
    if (cnt > 6)
        return old;
    return false;
}
