#include <algorithm>
#include <climits>
#include <iostream>
#include <random>

#pragma once

#define count(x) sizeof(x) / sizeof(x[0])
#define MAX_SIZE 1'000'000

size_t smallest_index (const int * arr, size_t count);

template<typename T> T my_random(T count) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<T> uniform_dist(0, count - 1);
    return uniform_dist(e1);
}

// Insertion sort
void sort_array (int * arr, size_t count);

// Merge sort
void sort_merge (int * arr, size_t count);

// Quick sort
void sort_quick (int * arr, size_t count);

bool is_almost_median(const int * arr, size_t count, int pivot);

void print_array (int * arr, size_t count);
