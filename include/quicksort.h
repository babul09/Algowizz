#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "visualization_state.h" // Include the common state header

// No specific data struct needed here anymore if managed by VisualizationState

#ifdef __cplusplus
extern "C" { // Keep C linkage if quicksort.c is C, but we'll make it C++
#endif

// Helper function for partition (internal logic, might not need external declaration)
// int partition(std::vector<int>& arr, int low, int high, VisualizationState& qsData);

// Step function is now declared in visualization_state.h
// bool StepQuickSort(VisualizationState& state);

#ifdef __cplusplus
}
#endif

#endif // QUICKSORT_H