#include <string>

#include "raylib.h"

int main() {
    const int windowWidth = 1080;
    const int windowHeight = 720;

    const float movementSpeed = 75.0f;
    bool debug = false;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "Resize Window");
    SetWindowMinSize(320, 240);

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Load textures
    Texture2D sheet = LoadTexture("../sheet.png");
    Rectangle player = {25, 25, 16, 16};
    Rectangle playerHitbox = {0, 0, 8, 15};

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();

        // Update
        if (IsKeyDown(KEY_D)) {
            player.x += movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_A)) {
            player.x -= movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_S)) {
            player.y += movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_W)) {
            player.y -= movementSpeed * deltaTime;
        }
        if (IsKeyPressed(KEY_F3)) {
            debug = !debug;
        }

        playerHitbox.x = player.x + 4;
        playerHitbox.y = player.y + 1;

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){player.x + player.width / 2, player.y + player.height / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawCircle(100, 75, 12.5, RED);
        if(CheckCollisionCircleRec((Vector2){100,75}, 12.5, playerHitbox)) {
            DrawText("Skibidi", 25,25,5, WHITE);
        }

        DrawRectanglePro(playerHitbox, (Vector2){0, 0}, 0, YELLOW);
        DrawTexturePro(sheet, (Rectangle){16.0001, 0, 15.9999, 15.9999}, player, (Vector2){0, 0}, 0, WHITE);

        EndMode2D();

        // Debug
        if (debug) {
            DrawFPS(0, 0);
            DrawText(("Position: " + std::to_string(player.x) + "," + std::to_string(player.y)).c_str(), 0, 16, 20, WHITE);
        }
        EndDrawing();
    }


    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
