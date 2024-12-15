#include <iostream>
#include <vector>
#include "search.h"

using namespace std;

int linear_search(const vector<int>& arr, int a)
{
    int n = arr.size();
    int i=0;
    while((arr[i] != a)  && (i < n))
    {
        i = i + 1;
    }
    if (i==n)
    {
        i = -1;
    }
    return i;
}

int linear_barrier_search(const vector<int>& arr, int a)
{
    int i=0;
    arr.push_back(a);// выдавало ошбку, что-то нажал, перестало выдавать
    while(arr != a)
    {
        i = i + 1
    }
    if (i==n)
    {
        i = -1;
    }
    return i;
}

int binary_search(const vector<int>& arr, int a)
{
    int n = arr.size();
    arr.sort();
    left = 0;
    right = n - 1;
    if (arr[0] == a)
    {
        return 0;
    }
    if (arr[n-1] == a)
    {
        return n-1;
    }
    
    while (right - left > 1)
    {
        n = n / 2;
        if arr[left + n] == a
        {
            return left + n ;
        }
        if arr[left + n] < a
        {
            left = left + n;
        }
        if arr[left + n] > a
        {
            right = left + n;
        }

    }

    return -1;

}


int sorted_linear_search(const vector<int>& arr, int a)
{
    arr.sort()
    int n = arr.size();
    int i=0;
    while((arr[i] != a)  && (i < n))
    {
        i = i + 1;
    }
    if (i==n)
    {
        i = -1;
    }
    return i;
}

