#include "insertionsort.h"
#include <algorithm> // for std::swap (or just manual swap)

// primaryIndex = i (main loop index, end of sorted portion)
// secondaryIndex = j (comparison index moving backwards)
// tertiaryIndex = key (value being inserted) - store index instead

bool StepInsertionSort(VisualizationState& state) {
    if (state.primaryIndex == -1) { // Initialize loops
        state.primaryIndex = 1; // i starts from 1
        state.tertiaryIndex = state.array[1]; // Store the key value conceptually
        state.secondaryIndex = 0; // j starts from i - 1
         state.highlightStart = 0; // Sorted portion
         state.highlightEnd = 0;
    }

    int i = state.primaryIndex;
    int j = state.secondaryIndex;
    int n = state.size;

    if (i >= n) {
        state.status = VIZ_STATE_FINISHED;
        state.primaryIndex = -1; // Reset indices
        state.secondaryIndex = -1;
        state.tertiaryIndex = -1;
        state.highlightStart = -1;
        state.highlightEnd = -1;
        return false; // Sort finished
    }

    // Insertion logic for one step
    // In a step-by-step manner, we handle one comparison or shift per step.
    // state.tertiaryIndex holds the *value* conceptually, but we need 'key' var locally.
    // Let's adjust state: primaryIndex = i, secondaryIndex = j, tertiaryIndex = index of key element

    if (state.secondaryIndex == -2) { // State indicating key is being prepared
        state.tertiaryIndex = i; // Index of the element to insert
        state.secondaryIndex = i - 1; // Start comparison from j = i - 1
         state.highlightStart = 0; // Mark sorted part
         state.highlightEnd = i-1;
    }

    j = state.secondaryIndex;
    int keyIndex = state.tertiaryIndex;
    int keyVal = state.array[keyIndex]; // Get value using index

    if (j >= 0 && state.array[j] > keyVal) {
        // Shift element
        state.array[j + 1] = state.array[j];
        state.secondaryIndex--; // Move j backwards for next comparison/shift
    } else {
        // Found insertion point or reached beginning
        state.array[j + 1] = keyVal; // Insert key

        // Move to the next element
        state.primaryIndex++;
        if (state.primaryIndex < n) {
             state.secondaryIndex = -2; // Signal to prepare next key
              state.highlightStart = 0;
              state.highlightEnd = state.primaryIndex-1; // Update sorted range visual
        } else {
             // Finished
             state.status = VIZ_STATE_FINISHED;
             state.primaryIndex = -1;
             state.secondaryIndex = -1;
             state.tertiaryIndex = -1;
              state.highlightStart = -1;
              state.highlightEnd = -1;
             return false;
        }
    }

    return true; // Still sorting or just finished the last step
}