#include "raylib.h"

int main() {
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
    Rectangle player = {100, 100, 64, 64};

    SetTargetFPS(60);

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update
        float movementSpeed = 200.0f;
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

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){player.x + player.width / 2, player.y + player.height / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawCircle(400, 300, 50, RED);
        if(CheckCollisionCircleRec((Vector2){400,300}, 50, player)) {
            DrawText("Skibidi", 100,100,20,WHITE);
        }
        DrawCircle(600, 500, 100, BLUE);
        DrawTexturePro(sheet, (Rectangle){16.0001, 0, 15.9999, 15.9999}, player, (Vector2){0, 0}, 0.0f, WHITE);
        EndMode2D();

        EndDrawing();
    }


    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
