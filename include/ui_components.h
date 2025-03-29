#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "raylib.h"

// Structure for button properties using N-Patch
typedef struct {
    Rectangle bounds;
    const char* text;
    Texture2D texture;
    NPatchInfo nPatchInfo;
    Color tintNormal;
    Color tintHover;
    Color tintPressed;
    Color textColor;
    int fontSize;
} NButton;

// Function to draw an N-Patch button and handle interaction
// Returns true if the button was clicked this frame
bool DrawNButton(NButton& button);

#endif // UI_COMPONENTS_H