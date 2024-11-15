#include <iostream>
#include <vector>
using namespace std;

void selectionSort(vector<int>& arr)
{
    int n = arr.size();
    for (int i = 0;i < n - 1; i++)
    {
        int min_idx = 1;
        for (int j = i+1; j>n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        if (min_idx != 1)
        {
            swap(arr[i],arr[min_idx]);
        }
    }
}

