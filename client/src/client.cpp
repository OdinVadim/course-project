#include <iostream>

using namespace std;

int main()
{
    cout << "It's client\n";

#ifdef UNIX
    cout << "On Unix\n";
#endif /*UNIX*/

#ifdef WIN
    cout << "On Windows\n";
#endif /*WIN*/

    return 0;
}