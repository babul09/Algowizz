#include "ui_components.h"
#include "raymath.h" // Required for CheckCollisionPointRec

bool DrawNButton(NButton& button) {
    Vector2 mousePos = GetMousePosition();
    bool clicked = false;
    Color currentTint = button.tintNormal;

    // Check interaction
    bool hovered = CheckCollisionPointRec(mousePos, button.bounds);
    bool pressed = hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool released = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    if (pressed) {
        currentTint = button.tintPressed;
    } else if (hovered) {
        currentTint = button.tintHover;
    }

    if (released) {
        clicked = true;
    }

    // Draw the button background using N-Patch
    DrawTextureNPatch(button.texture, button.nPatchInfo, button.bounds, Vector2Zero(), 0.0f, currentTint);

    // Draw the text centered
    float textWidth = MeasureText(button.text, button.fontSize);
    Vector2 textPosition = {
        button.bounds.x + (button.bounds.width - textWidth) / 2.0f,
        button.bounds.y + (button.bounds.height - button.fontSize) / 2.0f
    };

    DrawText(button.text, (int)textPosition.x, (int)textPosition.y, button.fontSize, button.textColor);

    return clicked;
}