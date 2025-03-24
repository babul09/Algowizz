#include "quicksort.h"
#include "raylib.h"

// Function to partition the array for Quicksort
int partition(int* arr, int low, int high, QuickSortData* qsData) {
    int pivot = arr[high];
    int i = (low - 1);

    qsData->pivot = high;
    qsData->i = i;
    qsData->j = low;

    for (int j = low; j <= high - 1; j++) {
        qsData->j = j;
        qsData->delayTimer = 0; // Reset delay for each comparison
        while (qsData->delayTimer < qsData->delayAmount) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Quicksort Visualization", 20, 20, 20, WHITE);
            DrawText("Press BACKSPACE to go back to the menu", 20, 50, 15, GRAY);
            // Draw the array elements as bars
            float barWidth = (float)GetScreenWidth() / qsData->size;
            float maxHeight = GetScreenHeight() - 150;
            for (int k = 0; k < qsData->size; k++) {
                float barHeight = (float)qsData->array[k] / 100 * maxHeight; // Assuming values are within a reasonable range
                Color barColor = WHITE;
                if (k == high) barColor = YELLOW; // Pivot
                else if (k == qsData->j) barColor = RED; // Current j
                else if (k == qsData->i) barColor = GREEN; // Current i

                DrawRectangle(k * barWidth, GetScreenHeight() - 100 - barHeight, barWidth - 2, barHeight, barColor);
            }
            EndDrawing();
            qsData->delayTimer++;
        }
        if (arr[j] < pivot) {
            i++;
            qsData->i = i;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// Quicksort algorithm
void quickSort(int* arr, int low, int high, QuickSortData* qsData) {
    if (low < high) {
        int pi = partition(arr, low, high, qsData);

        // Recursively sort elements before and after pivot
        quickSort(arr, low, pi - 1, qsData);
        quickSort(arr, pi + 1, high, qsData);
    }
}