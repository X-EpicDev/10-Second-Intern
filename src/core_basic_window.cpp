#include "raylib.h"
#include "raymath.h"        // Required for: Vector2Clamp()

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(void) {
    const int windowWidth = 1080;
    const int windowHeight = 720;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "Resize Window");
    SetWindowMinSize(320, 240);

    int gameScreenWidth = 1080;
    int gameScreenHeight = 720;

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    Texture2D sheet = LoadTexture("../sheet.png");
    Rectangle playerDestRect = {100, 100, 64, 64};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        if (IsKeyDown('D')) {
            playerDestRect.x += 100 * GetFrameTime();
        } else if (IsKeyDown('A')) {
            playerDestRect.x -= 100 * GetFrameTime();
        }

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {0};
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){0, 0},
                                    (Vector2){(float) gameScreenWidth, (float) gameScreenHeight});

        //drawing
        BeginDrawing();
        ClearBackground(BLACK); // Clear screen background

        // Circle Drawing
        DrawCircle((GetScreenWidth() - ((float) 100 * scale)) * 0.5f,(GetScreenHeight() - ((float) 100 * scale)) * 0.5f, 100, RED);
        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 100, BLUE);
        DrawTexturePro(sheet, {16, 0, 16, 16}, playerDestRect, {0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
