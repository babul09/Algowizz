#ifndef VISUALIZATION_STATE_H
#define VISUALIZATION_STATE_H

#include "raylib.h"
#include <vector>

// Enum for the current state of the visualization
typedef enum {
    VIZ_STATE_IDLE,
    VIZ_STATE_SORTING,
    VIZ_STATE_PAUSED,
    VIZ_STATE_FINISHED
} VisualizationStatus;

// Enum for the algorithm type
typedef enum {
    ALGO_NONE,
    ALGO_QUICKSORT,
    ALGO_BUBBLESORT,
    ALGO_INSERTIONSORT
    // Add other algorithms here
} AlgorithmType;

// Structure to hold common visualization data and controls
typedef struct {
    std::vector<int> array; // Use std::vector for easier management
    int size;
    VisualizationStatus status;
    AlgorithmType currentAlgorithm;

    // Control parameters
    float speed; // Steps per second
    float timeAccumulator; // Time since last step
    bool stepMode; // If true, only advance one step on button press

    // Algorithm-specific state indices (example, adapt as needed)
    int primaryIndex;   // e.g., i in bubble/insertion sort, partition index in quicksort
    int secondaryIndex; // e.g., j in bubble/insertion sort, comparison index
    int tertiaryIndex;  // e.g., pivot index in quicksort
    int highlightStart; // Range highlighting
    int highlightEnd;

    // For Quicksort's recursive nature (simplified state for this example)
    struct QuickSortStackFrame {
        int low;
        int high;
        int stage; // 0: initial, 1: partitioning, 2: after partition (before left call), 3: after left call (before right call), 4: done
        int partitionIndex; // Store result of partition
    };
    std::vector<QuickSortStackFrame> quickSortStack;


} VisualizationState;

// --- Function Prototypes ---

// Initialize the state with a random array
void InitializeVisualizationState(VisualizationState& state, int arraySize);

// Reset the array and state
void ResetVisualizationState(VisualizationState& state);

// Update the visualization (advances one step if needed)
void UpdateVisualization(VisualizationState& state, float deltaTime);

// Draw the visualization panel (bars, indices)
void DrawVisualizationPanel(const VisualizationState& state, Rectangle bounds);

// Draw the control panel (buttons, sliders)
void DrawControlPanel(VisualizationState& state, Rectangle bounds, Texture2D buttonTexture, NPatchInfo buttonNpatchInfo);

// --- Algorithm Step Functions (to be called by UpdateVisualization) ---
// Returns true if the algorithm is still running
bool StepQuickSort(VisualizationState& state);
bool StepBubbleSort(VisualizationState& state);
bool StepInsertionSort(VisualizationState& state);

#endif // VISUALIZATION_STATE_H