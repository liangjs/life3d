#include <cstdlib>
#include <cstdio>
#include <vector>
using namespace std;
vector<int> a;
int main()
{
    a.resize(100);
    printf("%d\n",sizeof(a));
    return 0;
}
