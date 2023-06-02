#include "sort.h"

using namespace std;

int main(int argc, char ** argv) {
    // int arr[] = {16, 50, 8, 3, 56, 23, 15};
    int arr [MAX_SIZE];
    for (int & i : arr)
        i = my_random<int>(INT_MAX);

    if (argc < 2)
        return cout << "Usage: ./program <a|m|q|s>" << endl, 1;

    switch (argv[1][0]) {
        case 'a':
            sort_array(arr, count(arr));
            break;
        case 'm':
            sort_merge(arr, count(arr));
            break;
        case 'q':
            sort_quick(arr, count(arr));
            break;
        case 's':
            std::sort(arr, arr + count(arr));
            break;
    }

    cout << "Sorted." << endl;
    return 0;
}
