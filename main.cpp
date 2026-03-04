#include "BST.h"
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

void sieve_comparison(int n)
{
    const auto start_arr = chrono::high_resolution_clock::now();
    vector<bool> arr(n + 1, true);
    arr[0] = arr[1] = false;
    for (int p = 2; p * p <= n; p++)
    {
        if (arr[p])
        {
            for (int i = p * p; i <= n; i += p)
                arr[i] = false;
        }
    }
    const auto end_arr = chrono::high_resolution_clock::now();

    const auto start_bst = chrono::high_resolution_clock::now();
    BST<int> bst;
    for (int i = 2; i <= n; ++i) bst.insert(i);

    for (int p = 2; p * p <= n; p++)
    {
        if (bst.find(p) != bst.end())
        {
            for (int i = p * p; i <= n; i += p)
                bst.erase(bst.find(i));
        }
    }
    const auto end_bst = chrono::high_resolution_clock::now();

    cout << "Array time: " << chrono::duration<double>(end_arr - start_arr).count() << "s\n";
    cout << "BST time: " << chrono::duration<double>(end_bst - start_bst).count() << "s\n";
}

int main()
{
    sieve_comparison(100000);
    return 0;
}