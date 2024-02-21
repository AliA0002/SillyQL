// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <iostream>
#include "run.h"

using namespace std;
int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    cin >> std::boolalpha; 
    cout << std::boolalpha;
    Silly silly;
    silly.readOptions(argc, argv);
    silly.run();
    cout << "Thanks for being silly!\n";
    return 0;
}