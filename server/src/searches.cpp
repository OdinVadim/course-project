#include <iostream>
#include <vector>
#include "search.h"
#include <algorithm>

using namespace std;

struct element
{
    int value;
    int key;

    bool operator<(const element& a)
    {
        return (this->value < a.value);
    }
};

int linear_search(const vector<int>& arr, int a)
{
    int n = arr.size();
    int i = 0;
    for (i; i < n; i++)
    {
        if (arr[i] == a)
        {
            return i;
        }
    }
    return -1;
}

int linear_barrier_search( vector<int>& arr, int a)
{
    int i = 0;
    int n = arr.size();
    arr.push_back(a);// выдавало ошбку, что-то нажал, перестало выдавать
    while (arr[i] != a)
    {
        i = i + 1;
    }
    if (i == n)
    {
        i = -1;
    }
    return i;
}

int binary_search(vector<int>& arr_, int a)
{
    vector<element> arr = vector<element>(arr_.size());

    for (int i = 0; i < arr.size(); i++)
    {
        arr[i].value = arr_[i];
        arr[i].key = i;
    }
    sort(arr.begin(), arr.end());

    int left = 0;
    int right = arr.size() - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;

        if (arr[middle].value == a)
        {
            return arr[middle].key;
        }
        else if (arr[middle].value < a)
        {
            left = middle + 1;
        }
        else if (arr[middle].value > a)
        {
            right = middle - 1;
        }

    }

    return -1;

}


int sorted_linear_search(vector<int>& arr_, int a)
{
    vector<element> arr = vector<element>(arr_.size());

    for (int i = 0; i < arr.size(); i++)
    {
        arr[i].value = arr_[i];
        arr[i].key = i;
    }
    sort(arr.begin(),arr.end());
    int n = arr.size();
    int i = 0;
    for (i; i < n; i++)
    {
        if (arr[i].value == a)
        {
            return arr[i].key;
        }
    }
    return -1;
}
