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

    Camera2D camera;
    camera.target = (Vector2){playerDestRect.x + playerDestRect.width / 2, playerDestRect.y + playerDestRect.height / 2};
    camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        float movementSpeed = 200.0f;
        if (IsKeyDown('D')) {
            playerDestRect.x += movementSpeed * deltaTime;
        }
        if (IsKeyDown('A')) {
            playerDestRect.x -= movementSpeed * deltaTime;
        }
        if (IsKeyDown('S')) {
            playerDestRect.y += movementSpeed * deltaTime;
        }
        if (IsKeyDown('W')) {
            playerDestRect.y -= movementSpeed * deltaTime;
        }

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){playerDestRect.x + playerDestRect.width / 2, playerDestRect.y + playerDestRect.height / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawCircle(400, 300, 50, RED);
        DrawCircle(600, 500, 100, BLUE);
        DrawTexturePro(sheet, (Rectangle){16.0001, 0, 15.9999, 15.9999}, playerDestRect, (Vector2){0, 0}, 0.0f, WHITE);
        EndMode2D();

        EndDrawing();
    }


    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
