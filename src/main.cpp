#include "raylib.h"


void DrawHUD(float pitchDeg, float rollDeg, float headingDeg)
{
    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    const Color HUD_COLOR = GREEN;

    // Center of the screen = “boresight”
    Vector2 center = { (float)w / 2.0f, (float)h / 2.0f };

    // 1) Horizon & pitch ladder using a 2D camera for roll
    Camera2D cam = { 0 };
    cam.target = (Vector2){ 0, 0 };              // world origin
    cam.offset = center;                         // origin is at screen center
    cam.rotation = -rollDeg;                     // rotate opposite to aircraft roll
    cam.zoom = 1.0f;

    BeginMode2D(cam);

    float pitchScale = 8.0f; // pixels per degree

    // Draw pitch ladder lines every 5 degrees
    for (int p = -90; p <= 90; p += 5) {
        float y = (pitchDeg - p) * pitchScale;   // move ladder up/down with pitch
        float halfWidth = (p % 10 == 0) ? 60.0f : 30.0f;

        Vector2 left  = { -halfWidth,  y };
        Vector2 right = {  halfWidth,  y };

        DrawLineEx(left, right, 2.0f, HUD_COLOR);

        if (p % 10 == 0 && p != 0) {
            DrawText(TextFormat("%d", p),
                     right.x + 5, y - 8, 16, HUD_COLOR);
            DrawText(TextFormat("%d", p),
                     left.x - 25, y - 8, 16, HUD_COLOR);
        }
    }

    // Horizon line at pitch 0
    DrawLineEx((Vector2){ -2000, (pitchDeg - 0) * pitchScale },
               (Vector2){  2000, (pitchDeg - 0) * pitchScale },
               3.0f, HUD_COLOR);

    EndMode2D();

    // 2) Heading tape (drawn in screen space, not rotated)
    float tapeY = h - 60.0f;
    DrawLine(0, tapeY, w, tapeY, HUD_COLOR);

    float headingScale = 5.0f; // pixels per degree
    for (int hdg = -180; hdg <= 180; hdg += 5) {
        float rel = (float)hdg - headingDeg;
        float x = center.x + rel * headingScale;

        if (x < 0 || x > w) continue;

        int len = (hdg % 10 == 0) ? 15 : 8;
        DrawLine(x, tapeY, x, tapeY - len, HUD_COLOR);

        if (hdg % 30 == 0)
        {
            DrawText(TextFormat("%d", (hdg + 360) % 360),
                     x - 10, tapeY - 30, 16, HUD_COLOR);
        }
    }

    // 3) Flight path marker (circle + wings) at screen center offset by velocities
    DrawCircleLines(center.x, center.y, 20, HUD_COLOR);
    DrawLineEx((Vector2){ center.x - 20, center.y },
               (Vector2){ center.x + 20, center.y }, 2, HUD_COLOR);
    DrawLineEx((Vector2){ center.x, center.y - 10 },
               (Vector2){ center.x, center.y + 10 }, 2, HUD_COLOR);
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);
    InitWindow(1920, 1080, "HUD");

    SetTargetFPS(60);
    float pitchDeg = 0.0f;
    float rollDeg = 0.0f;
    float headingDeg = 0.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        pitchDeg += 0.01f;
        rollDeg += 0.01f;
        headingDeg += 0.01f;
        ClearBackground(Color{0,0,0,0});
        DrawHUD(pitchDeg, rollDeg, headingDeg);
        EndDrawing();
    }
    CloseWindow();


    return 0;
}