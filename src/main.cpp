#include <string>

#include "Object.h"
#include "raylib.h"

int main() {
    const int windowWidth = 1080;
    const int windowHeight = 720;
    const float movementSpeed = 75.0f;
    bool debug = false;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Resize Window");
    SetWindowMinSize(320, 240);

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Load textures
    Texture2D sheet = LoadTexture("../sheet.png");
    Texture2D mapText = LoadTexture("../large office.png");

    Vector2 playerOffset{4,1};
    playerOffset.x = 4;
    playerOffset.y = 1;
    Object player(sheet, {16.0001, 0, 15.9999, 15.9999}, {25, 25, 16, 16}, {0, 0, 8, 15}, playerOffset);

    Vector2 mapOffset{0,0};
    mapOffset.x = 0;
    mapOffset.y = 0;
    Object map(mapText, {0,0,448,320}, {0,0,448,320}, {0,0,0,0}, mapOffset);

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();

        // Update
        if (IsKeyDown(KEY_D)) {
            player.setX(player.getX() + movementSpeed * deltaTime);
        }
        if (IsKeyDown(KEY_A)) {
            player.setX(player.getX() - movementSpeed * deltaTime);
        }
        if (IsKeyDown(KEY_S)) {
            player.setY(player.getY() + movementSpeed * deltaTime);
        }
        if (IsKeyDown(KEY_W)) {
            player.setY(player.getY() - movementSpeed * deltaTime);
        }
        if (IsKeyPressed(KEY_F3)) {
            debug = !debug;
        }

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){player.getX() + player.getWidth() / 2, player.getY() + player.getHeight() / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawTexturePro(mapText, map.getTextureArea(), map.getRectangle(), (Vector2){0,0}, 0, WHITE);

        DrawCircle(100, 75, 12.5, RED);
        if(CheckCollisionCircleRec((Vector2){100,75}, 12.5, player.getHitbox())) {
            DrawText("Skibidi", 25,25,5, WHITE);
        }

        player.draw();

        EndMode2D();

        // Debug
        if (debug) {
            DrawFPS(0, 0);
            DrawText(("Position: " + std::to_string(player.getX()) + "," + std::to_string(player.getY())).c_str(), 0, 16, 20, WHITE);
            DrawText(("DeltaTime: " + std::to_string(deltaTime)).c_str(), 0, 32, 20, WHITE);
        }
        EndDrawing();
    }


    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
