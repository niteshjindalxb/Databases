#include <iostream>
#include "extendibleHashing.h"
// #include "linearHashing.h"
using namespace std;

int main(int argc, char const *argv[])
{
/*
    class directory dir;

    dir.add_value(5);
    dir.add_value(9);
    dir.add_value(13);
    dir.add_value(17);
    dir.add_value(2);
    dir.add_value(6);
    dir.add_value(4);
    dir.add_value(8);
    dir.add_value(10);
    dir.add_value(0);
    dir.add_value(16);
    dir.add_value(1024);
    dir.add_value(0);
    dir.add_value(0);

    dir.print();

    cout << dir.search(15) << endl;
    cout << dir.search(16) << endl;
    cout << dir.search(1024) << endl;
    cout << dir.search(10) << endl;
    cout << dir.search(0) << endl;
*/
    
    // class linearHashing obj;
    class directory obj;
    for (int i=0; i<23; i++) {
        int val;
        cin >> val;
        obj.add_value(val);
    }
    obj.print();
    
    return 0;
}
