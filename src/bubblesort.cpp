#include "bubblesort.h"
#include <algorithm> // For std::swap

// Need to store state between steps. We use VisualizationState for this.
// Let's define primaryIndex = i (outer loop), secondaryIndex = j (inner loop)

bool StepBubbleSort(VisualizationState& state) {
    if (state.primaryIndex == -1) { // Initialize loops
        state.primaryIndex = 0; // i = 0
        state.secondaryIndex = 0; // j = 0
    }

    int i = state.primaryIndex;
    int j = state.secondaryIndex;
    int n = state.size;

    if (i >= n - 1) {
        state.status = VIZ_STATE_FINISHED;
        state.primaryIndex = -1; // Reset indices
        state.secondaryIndex = -1;
        return false; // Sort finished
    }

    // Perform one comparison/swap
    if (j < n - i - 1) {
        state.tertiaryIndex = j + 1; // Highlight comparison element
        if (state.array[j] > state.array[j + 1]) {
            std::swap(state.array[j], state.array[j + 1]);
            // Optionally add a flag for swap visualization
        }
        state.secondaryIndex++; // Move j forward
    } else {
        // Inner loop finished, move to next outer loop iteration
        state.primaryIndex++;
        state.secondaryIndex = 0; // Reset j
        state.tertiaryIndex = -1; // Clear comparison highlight
    }

    return true; // Still sorting
}