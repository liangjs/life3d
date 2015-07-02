#include "hash.h"
#include <cstring>
#include <cstdlib>

int hash(const Point &a)
{
    int f = a.x ^ (a.x >> 32);
    f ^= (f >> 23) ^ (f >> 17);
    f ^= a.y ^ (a.y >> 32);
    f ^= (f >> 15) ^ (f >> 7);
    f ^= a.z ^ (a.z >> 32);
    f ^= (f >> 5) ^ (f >> 3);
    f %= HASHP;
    if (f < 0)
        f += HASHP;
    return f;
}

class Pool
{
private:
    HashLink a[HASHP];
    HashPLink sta[HASHP];
    int top;
public:
    Pool()
    {
        top = HASHP;
        for (int i = 0; i < HASHP; ++i)
            sta[i] = a + i;
    }
    HashPLink get(const Point &p, HashPLink next)
    {
        if (top == 0)
        {
            HashPLink f = (HashPLink)malloc(sizeof(HashLink));
            f->a = p;
            f->next = next;
            return f;
        }
        else
        {
            HashPLink &tmp = sta[--top];
            tmp->a = p;
            tmp->next = next;
            return tmp;
        }
    }
    void put(HashPLink x)
    {
        if (top == HASHP)
            free(x);
        else
            sta[top++] = x;
    }
} pool;

class Pool2
{
private:
    HashLink2 a[HASHP];
    HashPLink2 sta[HASHP];
    int top;
public:
    Pool2()
    {
        top = HASHP;
        for (int i = 0; i < HASHP; ++i)
            sta[i] = a + i;
    }
    HashPLink2 get(const int x, HashPLink2 next)
    {
        if (top == 0)
        {
            HashPLink2 f = (HashPLink2)malloc(sizeof(HashLink2));
            f->a = x;
            f->next = next;
            return f;
        }
        else
        {
            HashPLink2 &tmp = sta[--top];
            tmp->a = x;
            tmp->next = next;
            return tmp;
        }
    }
    void put(HashPLink2 x)
    {
        if (top == HASHP)
            free(x);
        else
            sta[top++] = x;
    }
} pool2;

Hash::Hash()
{
    memset(a, 0, sizeof(a));
    al = NULL;
}

Hash::~Hash()
{
    HashPLink2 i = al, nx = i->next;
    while (i)
    {
        HashPLink j = a[i->a], next = j->next;
        while (j)
        {
            pool.put(j);
            j = next;
            next = next->next;
        }
        pool2.put(i);
        i = nx;
        nx = nx->next;
    }
}

void Hash::insert(const Point &p, int hashkey)
{
    if (hashkey == -1)
        hashkey = hash(p);
    if (a[hashkey] == NULL)
    {
        a[hashkey] = pool.get(p, NULL);
        al = pool2.get(hashkey, al);
    }
    else
    {
        for (HashPLink i = a[hashkey]; i; i = i->next)
            if (i->a == p)
                return;
        a[hashkey] = pool.get(p, a[hashkey]);
    }
}

bool Hash::have(const Point &p, int hashkey)
{
    if (hashkey == -1)
        hashkey = hash(p);
    for (HashPLink i = a[hashkey]; i; i = i->next)
        if (i->a == p)
            return true;
    return false;
}
