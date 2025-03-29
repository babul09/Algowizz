#include "raylib.h"
#include "resource_dir.h"   // utility header for SearchAndSetResourceDir
#include "visualization_state.h" // Include the new state management
#include "ui_components.h"     // Include the button component

#include <string> // For std::string

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
    SCREEN_MAIN_MENU,
    SCREEN_VISUALIZATION,
    SCREEN_SETTINGS // Added placeholder
    // Add other screens if needed
} GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 1200; // Increased width
static const int screenHeight = 800; // Increased height

static GameScreen currentScreen = SCREEN_MAIN_MENU;
static VisualizationState vizState = {}; // Global state for visualization

// UI Elements
static Texture2D buttonTexture;
static NPatchInfo buttonNpatchInfo;
static Font mainFont; // Optional: Load a custom font

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and Draw one frame

// Screen specific functions
static void UpdateMainMenuScreen(void);
static void DrawMainMenuScreen(void);
static void UpdateVisualizationScreen(void);
static void DrawVisualizationScreen(void);
static void UpdateSettingsScreen(void);
static void DrawSettingsScreen(void);

// Initialization
static void InitializeApp(void);
static void CleanupApp(void);


//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    InitializeApp();

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CleanupApp();
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void InitializeApp(void) {
     // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT); // Added Anti-aliasing hint

    // Create the window and OpenGL context
    InitWindow(screenWidth, screenHeight, "Algowizz++");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // Disable default ESC exit, handle manually

    // Utility function from resource_dir.h to find the resources folder
    SearchAndSetResourceDir("resources");

    // Load resources
    buttonTexture = LoadTexture("rec.png"); // Make sure rec.png is in resources
    if (buttonTexture.id == 0) {
        // Handle error - texture not loaded (draw default rects?)
        TraceLog(LOG_WARNING, "Failed to load button texture 'rec.png'");
        // Use basic colors if texture fails
    }

    // Define the N-Patch info for the button texture
    // Assuming rec.png is a simple 3x3 grid for stretching
    // Adjust these values based on your actual rec.png image borders
    buttonNpatchInfo = {
        (Rectangle){ 0.0f, 0.0f, (float)buttonTexture.width, (float)buttonTexture.height },
        8, 8, 8, 8, // Left, top, right, bottom border sizes in pixels
        NPATCH_NINE_PATCH // Layout type
    };

    // Optional: Load custom font
    // mainFont = LoadFont("your_font.ttf"); // Make sure font is in resources

    // Initialize Visualization State
    InitializeVisualizationState(vizState, 50); // Default size 50
}

void CleanupApp(void) {
    UnloadTexture(buttonTexture);
    // UnloadFont(mainFont); // If loaded
    if (!vizState.array.empty()) {
        // vector manages its own memory, no MemFree needed unless using raw pointers
    }
    CloseWindow();
}


void UpdateDrawFrame(void)
{
    // Update based on the current screen
    switch (currentScreen) {
        case SCREEN_MAIN_MENU:
            UpdateMainMenuScreen();
            break;
        case SCREEN_VISUALIZATION:
            UpdateVisualizationScreen();
            break;
        case SCREEN_SETTINGS:
             UpdateSettingsScreen();
             break;
        default: break;
    }

    // Draw
    BeginDrawing();
    ClearBackground(DARKGRAY); // Use a slightly less harsh background

    switch (currentScreen) {
        case SCREEN_MAIN_MENU:
            DrawMainMenuScreen();
            break;
        case SCREEN_VISUALIZATION:
            DrawVisualizationScreen();
            break;
         case SCREEN_SETTINGS:
             DrawSettingsScreen();
             break;
        default: break;
    }

    // Optional: Draw FPS
    DrawFPS(10, 10);

    EndDrawing();
}

// --- Main Menu Screen ---
void UpdateMainMenuScreen(void) {
    // No per-frame update needed here unless adding animations
     if (IsKeyPressed(KEY_Q)) exit(0); // Quick exit with Q
}

void DrawMainMenuScreen(void) {
    // Title
    const char* title = "Algowizz++";
    int titleFontSize = 60;
    int titleWidth = MeasureText(title, titleFontSize); // Use mainFont if loaded
    DrawText(title, (screenWidth - titleWidth) / 2, 80, titleFontSize, WHITE); // Use mainFont if loaded

    // Button properties
    float buttonWidth = 250;
    float buttonHeight = 50;
    float buttonSpacing = 20;
    float startY = 200;
    float centerX = (float)screenWidth / 2;

    // Button Colors
    Color btnNormal = WHITE; // Tint for N-Patch
    Color btnHover = LIGHTGRAY;
    Color btnPressed = GRAY;
    Color textColor = BLACK;

    // Algorithm Buttons
    NButton quicksortButton = {
        { centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight },
        "Quicksort", buttonTexture, buttonNpatchInfo,
        btnNormal, btnHover, btnPressed, textColor, 20
    };
    if (DrawNButton(quicksortButton)) {
        vizState.currentAlgorithm = ALGO_QUICKSORT;
        ResetVisualizationState(vizState); // Prepare state for this algo
        currentScreen = SCREEN_VISUALIZATION;
    }

     NButton bubbleButton = {
         { centerX - buttonWidth / 2, startY + (buttonHeight + buttonSpacing) * 1, buttonWidth, buttonHeight },
         "Bubble Sort", buttonTexture, buttonNpatchInfo,
         btnNormal, btnHover, btnPressed, textColor, 20
     };
     if (DrawNButton(bubbleButton)) {
         vizState.currentAlgorithm = ALGO_BUBBLESORT;
         ResetVisualizationState(vizState);
         currentScreen = SCREEN_VISUALIZATION;
     }

      NButton insertionButton = {
         { centerX - buttonWidth / 2, startY + (buttonHeight + buttonSpacing) * 2, buttonWidth, buttonHeight },
         "Insertion Sort", buttonTexture, buttonNpatchInfo,
         btnNormal, btnHover, btnPressed, textColor, 20
     };
      if (DrawNButton(insertionButton)) {
          vizState.currentAlgorithm = ALGO_INSERTIONSORT;
          ResetVisualizationState(vizState);
          currentScreen = SCREEN_VISUALIZATION;
      }

    // Settings Button (placeholder)
     NButton settingsButton = {
         { centerX - buttonWidth / 2, startY + (buttonHeight + buttonSpacing) * 3, buttonWidth, buttonHeight },
         "Settings", buttonTexture, buttonNpatchInfo,
         btnNormal, btnHover, btnPressed, textColor, 20
     };
     if (DrawNButton(settingsButton)) {
         currentScreen = SCREEN_SETTINGS;
     }


    // Exit Button
    NButton exitButton = {
        { centerX - buttonWidth / 2, startY + (buttonHeight + buttonSpacing) * 4 + 40, buttonWidth, buttonHeight }, // Extra space before exit
        "Exit", buttonTexture, buttonNpatchInfo,
        btnNormal, {255, 100, 100, 255} , {200, 80, 80, 255}, textColor, 20 // Red hover/press for exit
    };
    if (DrawNButton(exitButton)) {
       CloseWindow(); // Trigger the main loop exit condition
       // Note: CleanupApp() will be called after the loop breaks
    }
}


// --- Visualization Screen ---
void UpdateVisualizationScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
        // Optional: Pause before going back?
        vizState.status = VIZ_STATE_IDLE;
        vizState.currentAlgorithm = ALGO_NONE; // Deselect algo when going back
        currentScreen = SCREEN_MAIN_MENU;
        return; // Prevent further updates this frame
    }

    // Update the core visualization state machine
    UpdateVisualization(vizState, GetFrameTime());
}

void DrawVisualizationScreen(void) {
    // Define areas
    Rectangle controlPanelRect = { 0, 0, (float)screenWidth, 60 }; // Top panel for controls
    Rectangle vizPanelRect = { 0, controlPanelRect.height, (float)screenWidth, (float)screenHeight - controlPanelRect.height }; // Rest of screen for bars

    // Draw Backgrounds for panels (optional)
    DrawRectangleRec(controlPanelRect, { 30, 30, 30, 255 }); // Darker background for controls
    DrawRectangleRec(vizPanelRect, { 50, 50, 50, 255 });     // Slightly lighter for viz area

    // Draw the visualization bars
    DrawVisualizationPanel(vizState, vizPanelRect);

    // Draw the control panel UI elements
    DrawControlPanel(vizState, controlPanelRect, buttonTexture, buttonNpatchInfo);

    // Draw Algorithm Title
    const char* algoTitle = "Select Algorithm";
    switch(vizState.currentAlgorithm) {
        case ALGO_QUICKSORT: algoTitle = "Quicksort"; break;
        case ALGO_BUBBLESORT: algoTitle = "Bubble Sort"; break;
        case ALGO_INSERTIONSORT: algoTitle = "Insertion Sort"; break;
        default: break;
    }
     DrawText(algoTitle, (int)controlPanelRect.x + 15, screenHeight - 30, 20, LIGHTGRAY); // Bottom left corner
     DrawText(TextFormat("Array Size: %d", vizState.size), screenWidth - 150, screenHeight - 30, 20, LIGHTGRAY); // Bottom Right

     // Draw instructions
     DrawText("ESC/Backspace: Back to Menu", 10, screenHeight - 50, 10, GRAY);
}

// --- Settings Screen (Placeholder) ---
void UpdateSettingsScreen(void) {
     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
         currentScreen = SCREEN_MAIN_MENU;
     }
    // Handle settings changes here (e.g., default array size)
}

void DrawSettingsScreen(void) {
    DrawText("Settings", (screenWidth - MeasureText("Settings", 40)) / 2, 100, 40, WHITE);
    DrawText("Nothing here yet!", (screenWidth - MeasureText("Nothing here yet!", 20)) / 2, 200, 20, LIGHTGRAY);
    DrawText("Press ESC or Backspace to return", (screenWidth - MeasureText("Press ESC or Backspace to return", 20)) / 2, 250, 20, GRAY);

    // Add sliders or buttons for settings later
}