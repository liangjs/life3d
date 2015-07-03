#ifndef CORE_H
#define CORE_H

#include <unordered_set>
#include <unordered_map>

struct Point
{
    long long x, y, z;
    Point() {}
    Point(long long _x, long long _y, long long _z)
        : x(_x), y(_y), z(_z) {}
    bool operator== (const Point &a)const
    {
        return x == a.x && y == a.y && z == a.z;
    }
};

struct HashPoint
{
    int operator()(const Point &a)const;
};

class Board
{
private:
    std::unordered_set<Point, HashPoint> *data, *tmp;
public:
    void run();
    bool getStatus(int cnt, int old);
};

#endif
