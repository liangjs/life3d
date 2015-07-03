#include "hash.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
Hash a;
int main()
{
    int cnt = 0;
    for (int x = 0; x < 215; ++x)
    for (int y = 34-215*2; y < 34-215; ++y)
    for (int z = 123123+215; z < 123123+215*2; ++z)
    //for (int i = 0; i < 1e7; ++i)
    {
        //int x = rand(), y = rand(), z = rand();
        Point p(x, y, z);
        int hashkey = hash(p);
        //printf("%d\n", hashkey);
        a.insert(p, hashkey);
        ++cnt;
    }
    printf("%d\n", cnt);
    int ans = 0;
    for (HashPLink2 i = a.al; i; i = i->next)
    {
        int cnt = 0;
        for (HashPLink j = a.a[i->a]; j; j = j->next)
            ++cnt;
        if (cnt > ans)
            ans = cnt;
    }
    printf("%d\n", ans);
    return 0;
}
