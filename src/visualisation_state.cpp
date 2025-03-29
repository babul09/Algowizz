#include "visualization_state.h"
#include <cstdlib> // For rand(), srand()
#include <ctime>   // For time()
#include <algorithm> // For std::swap, std::min/max if needed
#include "raymath.h" // For Lerp

// Constants for drawing
const int BAR_AREA_PADDING = 20;
const int MIN_BAR_HEIGHT = 2;
const Color BAR_DEFAULT_COLOR = LIGHTGRAY;
const Color BAR_HIGHLIGHT_PRIMARY = GREEN;     // i
const Color BAR_HIGHLIGHT_SECONDARY = RED;     // j
const Color BAR_HIGHLIGHT_TERTIARY = YELLOW;   // pivot / key
const Color BAR_HIGHLIGHT_RANGE = BLUE;        // Range for quicksort partition
const Color BAR_SORTED_COLOR = SKYBLUE;

void InitializeVisualizationState(VisualizationState& state, int arraySize) {
    srand(time(NULL));
    state.size = arraySize;
    state.array.resize(state.size);
    for (int i = 0; i < state.size; ++i) {
        state.array[i] = rand() % 100 + 5; // Values from 5 to 104 (to ensure min height)
    }
    state.status = VIZ_STATE_IDLE;
    state.currentAlgorithm = ALGO_NONE;
    state.speed = 5.0f; // Default steps per second
    state.timeAccumulator = 0.0f;
    state.stepMode = false;

    // Reset indices
    state.primaryIndex = -1;
    state.secondaryIndex = -1;
    state.tertiaryIndex = -1;
    state.highlightStart = -1;
    state.highlightEnd = -1;
    state.quickSortStack.clear();
}

void ResetVisualizationState(VisualizationState& state) {
    // Regenerate array
    srand(time(NULL)); // Re-seed if desired, or keep sequence
    for (int i = 0; i < state.size; ++i) {
        state.array[i] = rand() % 100 + 5;
    }
    state.status = VIZ_STATE_IDLE; // Ready to start again
    state.timeAccumulator = 0.0f;
    // Keep speed and stepMode settings

    // Reset indices and algorithm state
    state.primaryIndex = -1;
    state.secondaryIndex = -1;
    state.tertiaryIndex = -1;
    state.highlightStart = -1;
    state.highlightEnd = -1;
    state.quickSortStack.clear();

    // If an algorithm was selected, prepare it to start from beginning
    if (state.currentAlgorithm != ALGO_NONE) {
         state.status = VIZ_STATE_PAUSED; // Or IDLE, user presses play
        // Re-initialize specific algo state if needed (e.g., push initial Quicksort frame)
         if (state.currentAlgorithm == ALGO_QUICKSORT) {
             state.quickSortStack.clear();
             state.quickSortStack.push_back({0, state.size - 1, 0, -1});
             state.highlightStart = 0;
             state.highlightEnd = state.size -1;
         } else if (state.currentAlgorithm == ALGO_BUBBLESORT) {
              state.primaryIndex = 0; // Ready for first step
              state.secondaryIndex = 0;
         } else if (state.currentAlgorithm == ALGO_INSERTIONSORT) {
              state.primaryIndex = 1; // Ready for first step
              state.secondaryIndex = -2; // Signal key prep needed
              state.highlightStart = 0;
              state.highlightEnd = 0;
         }
    }
}


void UpdateVisualization(VisualizationState& state, float deltaTime) {
    if (state.status != VIZ_STATE_SORTING || state.currentAlgorithm == ALGO_NONE) {
        return; // Do nothing if not sorting or no algorithm selected
    }

    if (state.stepMode) {
        return; // In step mode, only advance via button press
    }

    state.timeAccumulator += deltaTime;
    float timePerStep = 1.0f / state.speed;

    bool stillRunning = true;
    while (state.timeAccumulator >= timePerStep && stillRunning && state.status == VIZ_STATE_SORTING) {
        state.timeAccumulator -= timePerStep;

        switch (state.currentAlgorithm) {
            case ALGO_QUICKSORT:
                stillRunning = StepQuickSort(state);
                break;
            case ALGO_BUBBLESORT:
                stillRunning = StepBubbleSort(state);
                break;
            case ALGO_INSERTIONSORT:
                stillRunning = StepInsertionSort(state);
                break;
            // Add cases for other algorithms
            default:
                stillRunning = false;
                break;
        }
         if (!stillRunning) {
             state.status = VIZ_STATE_FINISHED;
             // Clear highlights maybe? Or leave final state shown
                state.primaryIndex = -1;
                state.secondaryIndex = -1;
                state.tertiaryIndex = -1;
                state.highlightStart = -1;
                state.highlightEnd = -1;
         }
    }
     // If speed is very high, ensure accumulator doesn't grow indefinitely
     if (state.timeAccumulator > timePerStep * 2) {
         state.timeAccumulator = timePerStep * 2;
     }
}

void DrawVisualizationPanel(const VisualizationState& state, Rectangle bounds) {
    if (state.array.empty()) return;

    float panelWidth = bounds.width - 2 * BAR_AREA_PADDING;
    float panelHeight = bounds.height - 2 * BAR_AREA_PADDING;
    float barWidth = panelWidth / state.size;

    // Find max value for scaling (or assume 100-110 based on generation)
    int maxValue = 105; // Based on rand()%100 + 5
    // for (int val : state.array) { if (val > maxValue) maxValue = val; } // More robust if needed

    float startX = bounds.x + BAR_AREA_PADDING;
    float startY = bounds.y + bounds.height - BAR_AREA_PADDING; // Bottom edge

    for (int i = 0; i < state.size; ++i) {
        float barHeight = ((float)state.array[i] / maxValue) * panelHeight;
        if (barHeight < MIN_BAR_HEIGHT) barHeight = MIN_BAR_HEIGHT;

        Rectangle barRect = {
            startX + i * barWidth,
            startY - barHeight,
            barWidth - (state.size > 50 ? 1 : 2), // Add spacing between bars if many
            barHeight
        };

        Color barColor = BAR_DEFAULT_COLOR;
        bool isDefaultColor = true; 

        // Determine color based on state
        if (state.status == VIZ_STATE_FINISHED) {
            barColor = BAR_SORTED_COLOR;
            isDefaultColor = false;
       } else {
           // Specific highlights take precedence
           if (i == state.tertiaryIndex) {
                barColor = BAR_HIGHLIGHT_TERTIARY;
                isDefaultColor = false;
           }
           else if (i == state.secondaryIndex) {
                barColor = BAR_HIGHLIGHT_SECONDARY;
                 isDefaultColor = false;
           }
           else if (i == state.primaryIndex) {
                barColor = BAR_HIGHLIGHT_PRIMARY;
                 isDefaultColor = false;
           }
           // Range highlight (only apply if color is still default)
           else if (isDefaultColor && state.highlightStart != -1 && i >= state.highlightStart && i <= state.highlightEnd) {
                if (state.currentAlgorithm == ALGO_QUICKSORT) {
                    // Assign range color directly instead of blending incorrectly
                    barColor = BAR_HIGHLIGHT_RANGE;
                    isDefaultColor = false;
                } else if (state.currentAlgorithm == ALGO_INSERTIONSORT) {
                    // Assign sorted color directly
                    barColor = BAR_SORTED_COLOR; // Or a slightly different shade if desired
                    isDefaultColor = false;
                }
           }
            // Bubble sort sorted part highlight (only apply if color is still default)
            else if (isDefaultColor && state.currentAlgorithm == ALGO_BUBBLESORT && state.primaryIndex != -1 && i >= state.size - state.primaryIndex -1) {
                 // Assign sorted color directly
                 barColor = BAR_SORTED_COLOR; // Or a slightly different shade
                 isDefaultColor = false;
            }
       }


        DrawRectangleRec(barRect, barColor);
        // Optionally draw value text on top for small arrays
         if (state.size <= 20) {
             DrawText(TextFormat("%d", state.array[i]), (int)barRect.x, (int)(barRect.y - 15), 10, WHITE);
         }
    }
}

// Draw the control panel (buttons, sliders) - Implementation depends heavily on ui_components
// This is a basic example placeholder
#include "ui_components.h" // Ensure included

void DrawControlPanel(VisualizationState& state, Rectangle bounds, Texture2D buttonTexture, NPatchInfo buttonNpatchInfo) {
    // Layout buttons within the bounds
    float padding = 10;
    float buttonWidth = 100;
    float buttonHeight = 30;
    float sliderWidth = 150;
    float currentX = bounds.x + padding;
    float currentY = bounds.y + padding; // Center vertically maybe? currentY = bounds.y + (bounds.height - buttonHeight) / 2;


    // Play/Pause Button
    NButton playPauseButton = {
        { currentX, currentY, buttonWidth, buttonHeight },
        (state.status == VIZ_STATE_SORTING) ? "Pause" : "Play",
        buttonTexture, buttonNpatchInfo,
        GRAY, DARKGRAY, BLACK, WHITE, 20
    };
    if (DrawNButton(playPauseButton) && state.currentAlgorithm != ALGO_NONE && state.status != VIZ_STATE_FINISHED) {
        if (state.status == VIZ_STATE_SORTING) {
            state.status = VIZ_STATE_PAUSED;
            state.stepMode = false; // Ensure step mode is off when pausing normally
        } else {
            state.status = VIZ_STATE_SORTING;
            state.stepMode = false;
             // If was IDLE, need to init algo state
             if(state.currentAlgorithm == ALGO_QUICKSORT && state.quickSortStack.empty()) {
                 ResetVisualizationState(state); // This will set up the first frame
                 state.status = VIZ_STATE_SORTING;
             }
              else if(state.currentAlgorithm == ALGO_BUBBLESORT && state.primaryIndex == -1) {
                  ResetVisualizationState(state);
                  state.status = VIZ_STATE_SORTING;
              }
               else if(state.currentAlgorithm == ALGO_INSERTIONSORT && state.primaryIndex == -1) {
                   ResetVisualizationState(state);
                   state.status = VIZ_STATE_SORTING;
               }
        }
    }
    currentX += buttonWidth + padding;

    // Step Button
    NButton stepButton = {
        { currentX, currentY, buttonWidth, buttonHeight },
        "Step",
        buttonTexture, buttonNpatchInfo,
        GRAY, DARKGRAY, BLACK, WHITE, 20
    };
     if (DrawNButton(stepButton) && state.currentAlgorithm != ALGO_NONE && (state.status == VIZ_STATE_PAUSED || state.status == VIZ_STATE_IDLE)) {
        state.status = VIZ_STATE_SORTING; // Set to sorting to allow one step
        state.stepMode = true; // Enter step mode

         // Ensure algo state is initialized if starting from IDLE
         if(state.currentAlgorithm == ALGO_QUICKSORT && state.quickSortStack.empty()) ResetVisualizationState(state);
         else if(state.currentAlgorithm == ALGO_BUBBLESORT && state.primaryIndex == -1) ResetVisualizationState(state);
         else if(state.currentAlgorithm == ALGO_INSERTIONSORT && state.primaryIndex == -1) ResetVisualizationState(state);

         // Manually call the step function ONCE
         bool stillRunning = true;
         switch (state.currentAlgorithm) {
             case ALGO_QUICKSORT: stillRunning = StepQuickSort(state); break;
             case ALGO_BUBBLESORT: stillRunning = StepBubbleSort(state); break;
             case ALGO_INSERTIONSORT: stillRunning = StepInsertionSort(state); break;
             default: stillRunning = false; break;
         }

         if (!stillRunning) {
             state.status = VIZ_STATE_FINISHED;
             state.primaryIndex = -1; state.secondaryIndex = -1; state.tertiaryIndex = -1;
              state.highlightStart = -1; state.highlightEnd = -1;
         } else {
             state.status = VIZ_STATE_PAUSED; // Go back to paused after the step
         }
         // state.stepMode remains true until Play is pressed
     }
    currentX += buttonWidth + padding;


    // Reset Button
    NButton resetButton = {
        { currentX, currentY, buttonWidth, buttonHeight },
        "Reset",
        buttonTexture, buttonNpatchInfo,
        GRAY, DARKGRAY, BLACK, WHITE, 20
    };
    if (DrawNButton(resetButton)) {
        ResetVisualizationState(state); // Resets array and state, keeps current algo selected
    }
    currentX += buttonWidth + padding;

    // Speed Slider (Using basic Raylib rects for simplicity here)
    currentX += padding;
    Rectangle sliderRect = { currentX, currentY, sliderWidth, buttonHeight };
    DrawRectangleRec(sliderRect, DARKGRAY);
    float speedNormalized = Lerp(0.1f, 200.0f, state.speed / 200.0f); // Map speed to slider range (adjust max speed)
     // Non-linear mapping might be better (e.g., logarithmic)
     float sliderHandlePos = sliderRect.x + (state.speed / 200.0f) * sliderRect.width; // Linear for now


    DrawText("Speed", (int)sliderRect.x, (int)(sliderRect.y - 15), 10, WHITE);
    Rectangle handleRect = { sliderHandlePos - 5, currentY, 10, buttonHeight };
    DrawRectangleRec(handleRect, LIGHTGRAY);

    if (CheckCollisionPointRec(GetMousePosition(), sliderRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float mouseXRelative = GetMousePosition().x - sliderRect.x;
        state.speed = (mouseXRelative / sliderRect.width) * 200.0f; // Adjust max speed
        if (state.speed < 0.1f) state.speed = 0.1f; // Min speed
        if (state.speed > 200.0f) state.speed = 200.0f;
    }
     DrawText(TextFormat("%.1f steps/s", state.speed), (int)(sliderRect.x + sliderWidth + 5), (int)currentY + 10, 10, WHITE);
    currentX += sliderWidth + padding + 60; // Space for text


    // Status Text
    const char* statusText = "Status: IDLE";
    switch (state.status) {
        case VIZ_STATE_SORTING: statusText = state.stepMode ? "Status: STEP" : "Status: SORTING"; break;
        case VIZ_STATE_PAUSED:  statusText = "Status: PAUSED"; break;
        case VIZ_STATE_FINISHED:statusText = "Status: FINISHED"; break;
        case VIZ_STATE_IDLE: /* Default */ break;
    }
     DrawText(statusText, (int)currentX, (int)currentY + 10, 20, WHITE);

    // Back Button (example)
     NButton backButton = {
         { bounds.x + bounds.width - buttonWidth - padding, currentY, buttonWidth, buttonHeight },
         "Back",
         buttonTexture, buttonNpatchInfo,
         GRAY, DARKGRAY, BLACK, WHITE, 20
     };
     // We need a way to signal back to main.cpp. Let's return true if back is clicked.
     // Modify DrawControlPanel prototype if needed, or use a flag in the state.
     // For now, assume main.cpp checks for ESCAPE key.


}