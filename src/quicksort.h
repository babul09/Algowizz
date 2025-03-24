#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "raylib.h"

// Quicksort visualization data
typedef struct {
    int* array;
    int size;
    int pivot;
    int i;
    int j;
    bool sorting;
    int delayTimer;
    int delayAmount;
} QuickSortData;

#ifdef __cplusplus
extern "C" {
#endif

// Function prototype for partition
int partition(int* arr, int low, int high, QuickSortData* qsData);

// Function prototype for quicksort
void quickSort(int* arr, int low, int high, QuickSortData* qsData);

#ifdef __cplusplus
}
#endif

#endif // QUICKSORT_H