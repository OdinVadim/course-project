#include <iostream>
#include <vector>
#include "search.h"
#include <algorithm>
#include <map>
#include <string>

using namespace std;

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

int binary_search(vector<int>& arr, int a)
{
    std::sort(arr.begin(), arr.end(), ());
    int n = arr.size();
    int left = 0;
    int right = n - 1;
    if (arr[0] == a)
    {
        return 0;
    }
    if (arr[n - 1] == a)
    {
        return n - 1;
    }

    while (right - left > 1)
    {
        n = n / 2;
        if (arr[left + n] == a)
        {
            return left + n;
        }
        if (arr[left + n] < a)
        {
            left = left + n;
        }
        if (arr[left + n] > a)
        {
            right = left + n;
        }

    }

    return -1;

}


int sorted_linear_search(vector<int>& arr, int a)
{
    // std::map<std::string, int> slov;
    // int n = arr.size();
    // int i = 0;
    // for (i; i < n; i++)
    // {
    //     string string_i = to_string(i);
    //     slov[string_i] = arr[i];
    // }
    // std::vector<std::pair<int, std::string>> fr;
    // for (const auto& el : slov) {
    //    fr.push_back({el.second, el.first});
    // }
    // std::sort(fr.begin(), fr.end(), [](const auto& el1, const auto& el2) { return el1.first > el2.first;});
    
    int n = arr.size();
    int i = 0;
    struct data
    {
        float val;
        int index;
    };

    std::vector<data> vals(n);

    for (i; i < n; i++)
    {
        vals[i].val=arr[i];
        vals[i].index=i;
    }
    
}
