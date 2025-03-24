#include "raylib.h"
#include "quicksort.h" // Include the new header file
#include "resource_dir.h"   // utility header for SearchAndSetResourceDir
#include <stdlib.h>         // For rand(), srand()
#include <time.h>           // For time()

int main ()
{
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    const int screenWidth = 1000;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Algowizz");
    SetTargetFPS(60);

    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load textures
    Texture wabbit = LoadTexture("wabbit_alpha.png");
    Texture rec = LoadTexture("rec.png");

    // Define colors
    Color backgroundColor = BLACK;
    Color textColor = WHITE;
    Color buttonColorNormal = GRAY;
    Color buttonColorHover = DARKGRAY;

    // Menu title
    const char* menuTitle = "Welcome to Algowizz";
    int titleFontSize = 40;
    Vector2 titlePosition = { (float)screenWidth / 2 - MeasureText(menuTitle, titleFontSize) / 2, 50 };

    // Button dimensions and spacing
    float buttonWidth = 200;
    float buttonHeight = 60;
    float buttonSpacing = 20;
    float startY = 200;
    float centerX = (float)screenWidth / 2;

    // Button texts
    const char* buttonText1 = "Quicksort";
    const char* buttonText2 = "Algorithm 2";
    const char* buttonText3 = "Settings";
    const char* buttonTextExit = "Exit";

    // Button rectangles
    Rectangle buttonRect1 = { centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight };
    Rectangle buttonRect2 = { centerX - buttonWidth / 2, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
    Rectangle buttonRect3 = { centerX - buttonWidth / 2, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };
    Rectangle buttonRectExit = { centerX - buttonWidth / 2, startY + 3 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };

    // Game state
    int gameState = 0; // 0: main menu, 1: Quicksort, 2: Algorithm 2, 3: Settings

    // Quicksort data
    QuickSortData quickSortData = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        // Input handling
        Vector2 mousePosition = GetMousePosition();

        if (gameState == 0) // Main menu state
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mousePosition, buttonRect1))
                {
                    gameState = 1; // Go to Quicksort visualization
                    // Initialize quicksort data
                    srand(time(NULL));
                    quickSortData.size = 50;
                    quickSortData.array = (int*)MemAlloc(quickSortData.size * sizeof(int));
                    for (int i = 0; i < quickSortData.size; i++) {
                        quickSortData.array[i] = rand() % 100 + 1; // Random values between 1 and 100
                    }
                    quickSortData.sorting = true;
                    quickSortData.delayAmount = 5; // Adjust for visualization speed
                    quickSortData.delayTimer = 0;
                    quickSortData.pivot = -1;
                    quickSortData.i = -1;
                    quickSortData.j = -1;
                }
                else if (CheckCollisionPointRec(mousePosition, buttonRect2))
                {
                    gameState = 2; // Go to Algorithm 2 screen
                }
                else if (CheckCollisionPointRec(mousePosition, buttonRect3))
                {
                    gameState = 3; // Go to Settings screen
                }
                else if (CheckCollisionPointRec(mousePosition, buttonRectExit))
                {
                    break; // Exit the application
                }
            }
        }
        else if (gameState == 1) // Quicksort visualization
        {
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                gameState = 0; // Go back to the main menu
                MemFree(quickSortData.array); // Free allocated memory
                quickSortData.array = NULL; // Important to set to NULL after freeing
            }

            if (quickSortData.sorting && quickSortData.array != NULL) {
                static int low = 0;
                static int high = 0;
                static bool initialCall = true;

                if (initialCall) {
                    high = quickSortData.size - 1;
                    initialCall = false;
                }

                if (low < high) {
                    static int pi = -1;
                    if (pi == -1) {
                        pi = partition(quickSortData.array, low, high, &quickSortData);
                    }

                    static bool firstSubCall = true;
                    if (firstSubCall) {
                        firstSubCall = false;
                        quickSort(quickSortData.array, low, pi - 1, &quickSortData);
                    }

                    static bool secondSubCall = true;
                    if (secondSubCall) {
                        secondSubCall = false;
                        quickSort(quickSortData.array, pi + 1, high, &quickSortData);
                    }

                    // Reset for the next level of recursion
                    if (!firstSubCall && !secondSubCall) {
                        low++; // This logic needs refinement for proper recursive visualization
                        high--; // This logic needs refinement for proper recursive visualization
                        pi = -1;
                        firstSubCall = true;
                        secondSubCall = true;
                    }
                    if (low >= high) quickSortData.sorting = false; // Basic termination condition - needs better handling

                } else {
                    quickSortData.sorting = false;
                }
            }
        }
        else if (gameState > 1)
        {
            // Logic for other game states
            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = 0; // Go back to the main menu
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(backgroundColor);

        if (gameState == 0) // Main menu
        {
            DrawText(menuTitle, titlePosition.x, titlePosition.y, titleFontSize, textColor);

            Color btn1Color = CheckCollisionPointRec(mousePosition, buttonRect1) ? buttonColorHover : buttonColorNormal;
            DrawRectangleRec(buttonRect1, btn1Color);
            DrawText(buttonText1, buttonRect1.x + buttonWidth / 2 - MeasureText(buttonText1, 20) / 2, buttonRect1.y + buttonHeight / 2 - 10, 20, textColor);

            Color btn2Color = CheckCollisionPointRec(mousePosition, buttonRect2) ? buttonColorHover : buttonColorNormal;
            DrawRectangleRec(buttonRect2, btn2Color);
            DrawText(buttonText2, buttonRect2.x + buttonWidth / 2 - MeasureText(buttonText2, 20) / 2, buttonRect2.y + buttonHeight / 2 - 10, 20, textColor);

            Color btn3Color = CheckCollisionPointRec(mousePosition, buttonRect3) ? buttonColorHover : buttonColorNormal;
            DrawRectangleRec(buttonRect3, btn3Color);
            DrawText(buttonText3, buttonRect3.x + buttonWidth / 2 - MeasureText(buttonText3, 20) / 2, buttonRect3.y + buttonHeight / 2 - 10, 20, textColor);

            Color btnExitColor = CheckCollisionPointRec(mousePosition, buttonRectExit) ? buttonColorHover : buttonColorNormal;
            DrawRectangleRec(buttonRectExit, btnExitColor);
            DrawText(buttonTextExit, buttonRectExit.x + buttonWidth / 2 - MeasureText(buttonTextExit, 20) / 2, buttonRectExit.y + buttonHeight / 2 - 10, 20, textColor);
        }
        else if (gameState == 1) // Quicksort Visualization
        {
            DrawText("Quicksort Visualization", 20, 20, 20, textColor);
            DrawText("Press ESC to go back to the menu", 20, 50, 15, GRAY);

            if (quickSortData.array != NULL) {
                // Draw the array elements as bars
                float barWidth = (float)GetScreenWidth() / quickSortData.size;
                float maxHeight = GetScreenHeight() - 150;
                for (int i = 0; i < quickSortData.size; i++) {
                    float barHeight = (float)quickSortData.array[i] / 100 * maxHeight; // Assuming values are within a reasonable range
                    Color barColor = WHITE;
                    if (i == quickSortData.pivot) barColor = YELLOW;
                    else if (i == quickSortData.j) barColor = RED;
                    else if (i == quickSortData.i) barColor = GREEN;

                    DrawRectangle(i * barWidth, GetScreenHeight() - 100 - barHeight, barWidth - 2, barHeight, barColor);
                }
            }
        }
        else if (gameState == 2)
        {
            DrawText("Algorithm 2 Screen", 20, 20, 20, textColor);
            DrawText("Press ESC to go back to the menu", 20, 50, 15, GRAY);
            // Draw your Algorithm 2 visualization here
        }
        else if (gameState == 3)
        {
            DrawText("Settings Screen", 20, 20, 20, textColor);
            DrawText("Press ESC to go back to the menu", 20, 50, 15, GRAY);
            // Add your settings options here
        }

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(wabbit);
    UnloadTexture(rec);
    if (quickSortData.array != NULL) MemFree(quickSortData.array);
    CloseWindow();
    return 0;
}