#include <iostream>
#include <vector>

#include "time.h"
#include "sorts.h"

using namespace std;

void selection_sort(vector<int>& arr)
{   
    unsigned long long int swaps = 0;
    unsigned long long int compares = 0;
    time_t start, end;
    time(&start);
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        for (int j = i+1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
            compares += 2;
        }
        if (min_idx != i)
        {
            swap(arr[i],arr[min_idx]);
            swaps += 1;
        }
        compares += 2;
        if (n <= 15)
        {
            cout <<"[Info] " <<i+1 << ". ";
            for(int k = 0; k < n; k++)
            {
                cout << arr[k] << " ";
            }
            cout << endl;
        }
    }
    time(&end);
    double seconds = difftime(end, start);
    cout << "[Info] Number of permutations: " << swaps << endl;
    cout << "[Info] Number of comparisons: " << compares << endl;
    if (n > 15)
    {
        cout << "[Info] Time spent by the algorithm: " << seconds << " seconds" << endl;
    }
    
}

void shell_sort(vector<int>& arr)
{
    unsigned long long int swaps = 0;
    unsigned long long int compares = 0;
    time_t start, end;
    time(&start);
    int n = arr.size();
    int gap = n / 2;
    int numb = 0;
    while(gap > 0)
    {
        for (int i = gap; i < n; i++)
        {
            numb++;
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp)
            {
                arr[j] = arr[j-gap];
                j -= gap;
                compares += 2;
            }
            arr[j] = temp;
            if (n <= 15)
            {
                cout <<"[Info] " << numb << ". ";
                for(int k = 0; k < n; k++)
                {
                    cout << arr[k] << " ";
                }
                cout << endl;
            }
            swaps += 1;
            compares += 1;
            
        }
        gap /= 2;
        compares += 1;
    }
    time(&end);
    double seconds = difftime(end, start);
    cout << "[Info] Number of permutations: " << swaps << endl;
    cout << "[Info] Number of comparisons: " << compares << endl;
    if (n > 15)
    {
        cout << "[Info] Time spent by the algorithm: " << seconds << " seconds" << endl;
    }
}
