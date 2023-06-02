#include "sort.h"

using namespace std;

size_t smallest_index (const int * arr, size_t count) {
    size_t smallest = 0;
    for (size_t i = 1; i < count; ++i)
        if (arr[i] < arr[smallest])
            smallest = i;
    return smallest;
}

void sort_array (int * arr, size_t count) {
    if (!count)
        return;

    size_t ix = smallest_index(arr, count);
    swap(arr[0], arr[ix]);
    sort_array(arr + 1, count - 1);
}

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

bool is_almost_median(const int * arr, size_t count, int pivot) {
    size_t smaller = 0;
    for (size_t i = 0; i < count; ++i)
        if (arr[i] < pivot)
            smaller++;
    return smaller > (count / 2);
}

void print_array (int * arr, size_t count) {
    for (size_t i = 0; i < count; ++i)
        cout << arr[i] << ((i != count - 1) ? ", " : "");
    cout << endl;
}
