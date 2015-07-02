#ifndef HASH_H
#define HASH_H
#define HASHP 12048193
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
typedef struct HashLink *HashPLink;
struct HashLink
{
    Point a;
    HashPLink next;
};
typedef struct HashLink2 *HashPLink2;
struct HashLink2
{
    int a;
    HashPLink2 next;
};
struct Hash
{
    HashPLink a[HASHP];
    HashPLink2 al;
    Hash();
    ~Hash();
    void insert(const Point &p, int hashkey = -1);
    bool have(const Point &p, int hashkey = -1);
};
#endif
