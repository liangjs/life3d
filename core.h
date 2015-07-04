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
    bool getStatus(int cnt, bool old);
public:
    Board();
    void run();
    const std::unordered_set<Point, HashPoint> *getData() { return data; }
};

struct DPoint
{
    double x, y, z;
    DPoint(double _x = 0, double _y = 0, double _z = 0)
        : x(_x), y(_y), z(_z) {}
};

DPoint rotate(const DPoint &p, const DPoint &v, double angle);
DPoint cross(const DPoint &a, const DPoint &b);
double dot(const DPoint &a, const DPoint &b);
double length(const DPoint &a);
DPoint operator+ (const DPoint &a, const DPoint &b);
DPoint operator- (const DPoint &a, const DPoint &b);
DPoint operator* (const DPoint &a, double b);
DPoint operator/ (const DPoint &a, double b);
bool operator== (const DPoint &a, const DPoint &b);
bool operator!= (const DPoint &a, const DPoint &b);

int dcmp(double x, double y = 0);
#endif
