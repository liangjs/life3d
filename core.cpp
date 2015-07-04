#include "core.h"
#include <cstdlib>
#include <cmath>

int hash(const Point &a)
{
    int ax = a.x ^ (a.x >> 32);
    int ay = a.y ^ (a.y >> 32);
    int az = a.z ^ (a.z >> 32);
    int fx = (ax & 0xffff) ^ (ay >> 16);
    int fy = (ay & 0xffff) ^ (az >> 16);
    int fz = (az & 0xffff) ^ (ax >> 16);
    int f = (fx << 22) ^ (fy << 11) ^ fz;
    return (f % HASHP + HASHP) % HASHP;
}

void HashSet::clear()
{
    for (auto i = pos.begin(); i != pos.end(); ++i)
        a[*i].clear();
    pos.clear();
}

void HashSet::insert(const Point &p, int hashkey, bool tested)
{
    if (hashkey == -1)
        hashkey = hash(p);
    if (!tested)
        for (auto i = a[hashkey].begin(); i != a[hashkey].end(); ++i)
            if (*i == p)
                return;
    if (a[hashkey].empty())
        pos.push_back(hashkey);
    a[hashkey].push_back(p);
}

bool HashSet::have(const Point &p, int hashkey)
{
    if (hashkey == -1)
        hashkey = hash(p);
    for (auto i = a[hashkey].begin(); i != a[hashkey].end(); ++i)
        if (*i == p)
            return true;
    return false;
}

void HashMap::clear()
{
    for (auto i = pos.begin(); i != pos.end(); ++i)
        a[*i].clear();
    pos.clear();
}

Board::Board()
{
    data = new HashSet;
    tmp = new HashSet;
    mp = new HashMap;
    for (int i = -50; i <= 50; ++i)
    for (int j = -30; j <= 10; ++j)
    for (int k = -10; k <= 10; ++k)
        if (rand() % 2)
            data->insert(Point(i, j, k));
}

Board::~Board()
{
    delete data;
    delete tmp;
    delete mp;
}

static const int dir[27][3] =
{
    {0, 0, 0}, {0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, 1, 1}, {0, 1, -1}, {0, -1, 0}, {0, -1, 1}, {0, -1, -1}
    , {1, 0, 0}, {1, 0, 1}, {1, 0, -1}, {1, 1, 0}, {1, 1, 1}, {1, 1, -1}, {1, -1, 0}, {1, -1, 1}, {1, -1, -1}
    , { -1, 0, 0}, { -1, 0, 1}, { -1, 0, -1}, { -1, 1, 0}, { -1, 1, 1}, { -1, 1, -1}, { -1, -1, 0}, { -1, -1, 1}, { -1, -1, -1}
};

void Board::run()
{
    tmp->clear();
    mp->clear();
    for (auto i = data->pos.begin(); i != data->pos.end(); ++i)
        for (auto j = data->a[*i].begin(); j != data->a[*i].end(); ++j)
            for (int k = 1; k < 27; ++k)
            {
                Point p(j->x + dir[k][0], j->y + dir[k][1], j->z + dir[k][2]);
                int hashkey = hash(p);
                if (mp->a[hashkey].empty())
                    mp->pos.push_back(hashkey);
                bool done = false;
                for (auto it = mp->a[hashkey].begin(); it != mp->a[hashkey].end(); ++it)
                    if (it->first == p)
                    {
                        done = true;
                        ++it->second;
                        break;
                    }
                if (!done)
                    mp->a[hashkey].push_back(std::make_pair(p, 1));
            }
    for (auto i = mp->pos.begin(); i != mp->pos.end(); ++i)
        for (auto j = mp->a[*i].begin(); j != mp->a[*i].end(); ++j)
            if (getStatus(j->second, data->have(j->first, *i)))
                tmp->insert(j->first, *i, true);
    std::swap(tmp, data);
}

inline bool Board::getStatus(int cnt, bool old)
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

DPoint operator* (const DPoint &a, double b)
{
    return DPoint(a.x * b, a.y * b, a.z * b);
}

DPoint operator/ (const DPoint &a, double b)
{
    return DPoint(a.x / b, a.y / b, a.z / b);
}

bool operator== (const DPoint &a, const DPoint &b)
{
    return dcmp(a.x, b.x) == 0 && dcmp(a.y, b.y) == 0 && dcmp(a.z, b.z) == 0;
}

bool operator!= (const DPoint &a, const DPoint &b)
{
    return dcmp(a.x, b.x) || dcmp(a.y, b.y) || dcmp(a.z, b.z);
}

int dcmp(double x, double y)
{
    static const double eps = 1e-8;
    double t = x - y;
    return (t > eps) - (t < -eps);
}
