#include "quicksort.h"
#include <vector>
#include <algorithm> // For std::swap

// Internal Partition Logic (modifies state indices)
int partition(VisualizationState& state, int low, int high) {
    int pivotValue = state.array[high];
    int i = (low - 1); // Index of smaller element

    state.tertiaryIndex = high; // Highlight pivot
    state.highlightStart = low;
    state.highlightEnd = high;

    for (int j = low; j <= high - 1; j++) {
        state.primaryIndex = i; // Highlight i
        state.secondaryIndex = j; // Highlight j (comparison pointer)

        if (state.array[j] < pivotValue) {
            i++;
            std::swap(state.array[i], state.array[j]);
            // Update state immediately after swap for visualization
            state.primaryIndex = i; // Show updated i
            // Optional: Add a micro-pause or flag here if needed for slow-mo swap viz
        }
        // Need to yield control back to main loop for drawing here in a real step-by-step
        // For this simplified version, we complete the partition in one "step" phase
    }
    std::swap(state.array[i + 1], state.array[high]);
    return (i + 1);
}


// Step function for Quicksort using an explicit stack
bool StepQuickSort(VisualizationState& state) {
    if (state.quickSortStack.empty()) {
        state.status = VIZ_STATE_FINISHED;
        state.primaryIndex = -1;
        state.secondaryIndex = -1;
        state.tertiaryIndex = -1;
         state.highlightStart = -1;
         state.highlightEnd = -1;
        return false; // Sorting finished
    }

    // Get current task from stack
    VisualizationState::QuickSortStackFrame currentFrame = state.quickSortStack.back();
    // Don't pop yet, might need to update stage

    int low = currentFrame.low;
    int high = currentFrame.high;

    if (low < high) {
        if (currentFrame.stage == 0) { // Initial call for this range
             state.quickSortStack.back().stage = 1; // Move to partitioning stage
            // Partitioning logic happens here conceptually.
            // We'll call our internal partition function which runs to completion
            // for this simplified step model.
            state.quickSortStack.back().partitionIndex = partition(state, low, high);
            state.quickSortStack.back().stage = 2; // Move to stage after partition
            return true; // Step completed (partition)
        }
        else if (currentFrame.stage == 2) { // After partition, push left sub-problem
             state.quickSortStack.back().stage = 3; // Mark current frame ready for right call later
            int pi = currentFrame.partitionIndex;
            if (low < pi - 1) {
                state.quickSortStack.push_back({low, pi - 1, 0, -1}); // Push left task
            }
             return true; // Step completed (pushed left task or skipped)
        }
         else if (currentFrame.stage == 3) { // After left sub-problem is handled (implicitly by stack), push right
             state.quickSortStack.back().stage = 4; // Mark current frame fully processed
             int pi = currentFrame.partitionIndex;
             if (pi + 1 < high) {
                 state.quickSortStack.push_back({pi + 1, high, 0, -1}); // Push right task
             }
             // Since stage is now 4, this frame will be popped next iteration if stack not empty
              return true; // Step completed (pushed right task or skipped)
         } else if (currentFrame.stage == 4) { // Frame done, pop it
             state.quickSortStack.pop_back();
             // Reset highlights from this frame if needed
             if (!state.quickSortStack.empty()) {
                 // Optionally set highlights based on the new top frame
                 state.highlightStart = state.quickSortStack.back().low;
                 state.highlightEnd = state.quickSortStack.back().high;
             } else {
                 state.highlightStart = -1;
                 state.highlightEnd = -1;
             }
             return true; // Step completed (popped frame)
         }

    } else {
        // low >= high, base case for this frame
        state.quickSortStack.pop_back(); // Pop this frame
         if (!state.quickSortStack.empty()) {
             state.highlightStart = state.quickSortStack.back().low;
             state.highlightEnd = state.quickSortStack.back().high;
         } else {
             state.highlightStart = -1;
             state.highlightEnd = -1;
         }
        return true; // Step completed (popped base case frame)
    }
     return true; // Should have returned earlier
}