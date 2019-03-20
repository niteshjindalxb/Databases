#include <iostream>
#include "extendibleHashing.h"
using namespace std;

int main(int argc, char const *argv[])
{
    class directory dir;
    for (int i = 0; i < 10; i++)
    {
        dir.add_value(i);
        // cout << i << " : " << dir.hash(i, 2) << endl; 
        // cout << i << endl;
    }
    dir.print();
    return 0;
}
