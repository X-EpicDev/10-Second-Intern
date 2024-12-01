#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "Object.h"
#include "raylib.h"

enum GameState {
    WAITING,
    PLAYING,
    FINISHED
};

int main() {
    const int windowWidth = 1080;
    const int windowHeight = 720;
    const float movementSpeed = 75.0f;
    bool debug = false;

    GameState gameState = WAITING;
    float timer = 10;
    int score = 0;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Resize Window");
    SetWindowMinSize(320, 240);

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Load textures
    Texture2D sheet = LoadTexture("../assets/sheet.png");
    Texture2D mapText = LoadTexture("../assets/office.png");

    // Text
    std::string startText = "Press SPACE or ENTER to begin";
    int startTextWidth = MeasureText(startText.c_str(), 20);

    std::string gameOverText = "Game Over";
    int gameOverTextWidth = MeasureText(gameOverText.c_str(), 40);

    // Objects
    Object player(sheet, {16.0001, 0, 15.9999, 15.9999}, {25, 25, 16, 16}, {0, 0, 8, 7}, Vector2{4, 9});
    Object map(mapText, {0, 0, 448, 320}, {0, 0, 448, 320}, {0, 0, 0, 0}, Vector2{0, 0});

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_F3)) {
            debug = !debug;
        }

        // Update
        switch (gameState) {
            case WAITING:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    gameState = PLAYING;
                }
                break;
            case PLAYING:
                timer -= deltaTime;
                if (timer <= 0) {
                    timer = 0;
                    gameState = FINISHED;
                    break;
                }

                if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                    player.setX(player.getX() + movementSpeed * deltaTime);
                }
                if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                    player.setX(player.getX() - movementSpeed * deltaTime);
                }
                if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                    player.setY(player.getY() + movementSpeed * deltaTime);
                }
                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                    player.setY(player.getY() - movementSpeed * deltaTime);
                }
                if (IsKeyPressed(KEY_E)) {
                    timer = 10;
                }
                break;
            case FINISHED:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    timer = 10;
                    score = 0;
                    player.setX(25);
                    player.setY(25);
                    gameState = PLAYING;
                }
                break;
        }

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){player.getX() + player.getWidth() / 2, player.getY() + player.getHeight() / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        map.draw();

        DrawCircle(100, 75, 12.5, RED);
        if (CheckCollisionCircleRec((Vector2){100, 75}, 12.5, player.getHitbox())) {
            DrawText("Skibidi", 25, 25, 5, WHITE);
        }

        player.draw();

        EndMode2D();

        std::ostringstream stream;
        switch (gameState) {
            case WAITING:
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, GetScreenHeight() / 5 * 3.5f, 20, WHITE);
                break;
            case FINISHED:
                DrawText(gameOverText.c_str(), GetScreenWidth() / 2 - gameOverTextWidth / 2, GetScreenHeight() / 3, 40, WHITE);
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, GetScreenHeight() / 5 * 3.5f, 20, WHITE);
                // No break so it still renders score/timer for now
            case PLAYING:
                DrawText(("Score: " + std::to_string(score)).c_str(), 0, 0, 20, WHITE);
                // Format to 1 decimal place
                stream << std::fixed << std::setprecision(1) << std::round(timer * 10) / 10.0;
                DrawText(("Time: " + stream.str()).c_str(), 0, 16, 20, WHITE);
                break;
        }

        // Debug
        if (debug) {
            DrawFPS(0, 32);
            DrawText(("Position: " + std::to_string(player.getX()) + "," + std::to_string(player.getY())).c_str(), 0, 48, 20, WHITE);
            DrawText(("DeltaTime: " + std::to_string(deltaTime)).c_str(), 0, 64, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
