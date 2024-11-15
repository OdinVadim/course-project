#include <iostream>
#include <vector>
#include "h.h"
#include "select.h"

using namespace std;

int main()
{
    cout << "It's server " << kafedra << "\n";
    vector<int> arr = {64, 25, 12, 22, 11};

    selectionSort(arr);

    cout <<"Sorted arr: ";
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}