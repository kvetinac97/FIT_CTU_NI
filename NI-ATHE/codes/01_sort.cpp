#include <algorithm>
#include <climits>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>

using namespace std;

#define count(x) sizeof(x) / sizeof(x[0])
#define MAX_SIZE 1'000'000

size_t smallest_index (const int * arr, size_t count) {
    size_t smallest = 0;
    for (size_t i = 1; i < count; ++i)
        if (arr[i] < arr[smallest])
            smallest = i;
    return smallest;
}

// Insertion sort
void sort_array (int * arr, size_t count) {
    if (!count)
        return;

    size_t ix = smallest_index(arr, count);
    swap(arr[0], arr[ix]);
    sort_array(arr + 1, count - 1);
}

// Merge sort
void sort_merge (int * arr, size_t count) {
    if (count <= 1)
        return;

    size_t middle = count / 2;
    sort_merge(arr, middle);
    sort_merge(arr + middle, count - middle);

    int * result = new int [MAX_SIZE];
    size_t i = 0, j = 0;
    while (i < middle && j < count - middle) {
        if (arr[i] <= arr[middle + j]) {
            result[i + j] = arr[i];
            i++;
        }
        else {
            result[j + i] = arr[middle + j];
            j++;
        }
    }
    while (i < middle) {
        result[i + j] = arr[i];
        i++;
    }
    while (j < count - middle) {
        result[j + i] = arr[middle + j];
        j++;
    }

    for (i = 0; i < count; ++i)
        arr[i] = result[i];

    delete [] result;
}

template<typename T> T my_random(T count) {
    random_device r;
    default_random_engine e1(r());
    uniform_int_distribution<T> uniform_dist(0, count - 1);
    return uniform_dist(e1);
}

bool is_almost_median(const int * arr, size_t count, int pivot) {
    size_t smaller = 0;
    for (size_t i = 0; i < count; ++i)
        if (arr[i] < pivot)
            smaller++;
    return smaller > (count / 2);
}

// Quick sort
void sort_quick (int * arr, size_t count) {
    if (count <= 1)
        return;

    if (count < 30)
        return sort_array(arr, count);

    size_t pivotIx;
    int pivot;
    do {
        pivotIx = my_random<size_t>(count);
        pivot = arr[pivotIx];
    }
    while (!is_almost_median(arr, count, pivot));


    size_t l_count = 0, r_count = 0;

    {
        int * left = new int [count], * right = new int [count];

        for (size_t i = 0; i < count; ++i) {
            if (i == pivotIx)
                continue;

            if (arr[i] < arr[pivotIx])
                left[l_count++] = arr[i];
            else
                right[r_count++] = arr[i];
        }

        for (size_t i = 0; i < count; ++i) {
            if (i == l_count) {
                arr[i] = pivot;
                continue;
            }

            if (i < l_count)
                arr[i] = left[i];
            else
                arr[i] = right[i - l_count - 1];
        }

        delete [] left;
        delete [] right;
    }

    sort_quick(arr, l_count);
    sort_quick(arr + l_count + 1, r_count);
}

void print_array (int * arr, size_t count) {
    for (size_t i = 0; i < count; ++i)
        cout << arr[i] << ((i != count - 1) ? ", " : "");
    cout << endl;
}

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
