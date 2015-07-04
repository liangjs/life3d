#ifndef CORE_H
#define CORE_H

#include <vector>
#include <utility>

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

int hash(const Point &a);

#define HASHP 1000037

struct HashSet
{
    std::vector<int> pos;
    std::vector<Point> a[HASHP];
    void clear();
    void insert(const Point &p, int hashkey = -1, bool tested = false);
    bool have(const Point &p, int hashkey = -1);
};

struct HashMap
{
    std::vector<int> pos;
    std::vector< std::pair<Point, int> > a[HASHP];
    void clear();
};

class Board
{
private:
    HashSet *data, *tmp;
    HashMap *mp;
    inline bool getStatus(int cnt, bool old);
public:
    Board();
    ~Board();
    void run();
    const HashSet *getData() { return data; }
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
