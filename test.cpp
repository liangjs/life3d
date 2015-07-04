#include <cstdlib>
#include <unordered_set>
using namespace std;
unordered_set<int> a;
int main()
{
    for (int i = 0; i < 1e6; ++i)
        a.insert(i);
    return 0;
}
