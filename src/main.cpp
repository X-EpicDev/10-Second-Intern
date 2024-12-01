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

    //top hitboxes
    Rectangle t1 = {16,15, 256, 1};
    Rectangle t2 = {288,15, 80, 1};
    Rectangle t3 = {176,111, 111, 1};
    Rectangle t4 = {16,159, 63, 1};
    Rectangle t5 = {97,159, 78, 1};
    Rectangle t6 = {273,159, 63, 1};
    Rectangle t7 = {352,159, 80, 1};
    Rectangle t8 = {273,239, 14, 1};
    Rectangle t9 = {16,223, 95, 1};
    Rectangle t10 = {129,223, 79, 1};
    Rectangle t11 = {288,223, 48, 1};
    Rectangle t12 = {273,239, 14, 1};
    Rectangle t13 = {337, 175, 14, 1};

    //bottom hitboxes
    Rectangle b1 = {161,104, 111, 1};
    Rectangle b2 = {16,152, 63, 1};
    Rectangle b3 = {97,152, 63, 1};
    Rectangle b4 = {273,152, 95, 1};
    Rectangle b5 = {337,200, 14, 1};
    Rectangle b6 = {16,216, 95, 1};
    Rectangle b7 = {129,216, 94, 1};
    Rectangle b8 = {273,216, 63, 1};
    Rectangle b9 = {273, 264, 14, 1};
    Rectangle b10 = {16, 312, 416, 1};
    //left hitboxes
    Rectangle l1 = {272,15,1,90};
    Rectangle l2 = {368,15,1,138};
    Rectangle l3 = {160,104,1,49};
    Rectangle l4 = {96,152,1,8};
    Rectangle l5 = {272,152,1,8};
    Rectangle l6 = {336,159,1,17};
    Rectangle l7 = {336,200,1,112};
    Rectangle l8 = {128,216,1,8};
    Rectangle l9 = {272,216,1,24};
    Rectangle l10 = {208,223,1,89};
    Rectangle l11 = {272,264,1,48};
    Rectangle l12 = {432, 159, 154, 1};

    //right hitboxes
    Rectangle r1 = {15,15,1,298};
    Rectangle r2 = {287,15,1,97};
    Rectangle r3 = {175,111,1,49};
    Rectangle r4 = {79,152,1,8};
    Rectangle r5 = {351,200,1,112};
    Rectangle r6 = {111,216,1,8};
    Rectangle r7 = {223,216,1,96};
    Rectangle r8 = {287,223,1,17};
    Rectangle r9 = {287,264,1,48};
    Rectangle r10 = {351,159,1,17};

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

        if(CheckCollisionRecs(r1, player.getHitbox())) {
            player.setX(player.getX() + movementSpeed * deltaTime);
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

        if(IsKeyDown(KEY_F4)) {
            DrawRectangleRec(t1, RED);
            DrawRectangleRec(t2, RED);
            DrawRectangleRec(t3, RED);
            DrawRectangleRec(t4, RED);
            DrawRectangleRec(t5, RED);
            DrawRectangleRec(t6, RED);
            DrawRectangleRec(t7, RED);
            DrawRectangleRec(t8, RED);
            DrawRectangleRec(t9, RED);
            DrawRectangleRec(t10, RED);
            DrawRectangleRec(t11, RED);
            DrawRectangleRec(t12, RED);
            DrawRectangleRec(t13, RED);

            DrawRectangleRec(b1, BLUE);
            DrawRectangleRec(b2, BLUE);
            DrawRectangleRec(b3, BLUE);
            DrawRectangleRec(b4, BLUE);
            DrawRectangleRec(b5, BLUE);
            DrawRectangleRec(b6, BLUE);
            DrawRectangleRec(b7, BLUE);
            DrawRectangleRec(b8, BLUE);
            DrawRectangleRec(b9, BLUE);
            DrawRectangleRec(b10, BLUE);

            DrawRectangleRec(l1, YELLOW);
            DrawRectangleRec(l2, YELLOW);
            DrawRectangleRec(l3, YELLOW);
            DrawRectangleRec(l4, YELLOW);
            DrawRectangleRec(l5, YELLOW);
            DrawRectangleRec(l6, YELLOW);
            DrawRectangleRec(l7, YELLOW);
            DrawRectangleRec(l8, YELLOW);
            DrawRectangleRec(l9, YELLOW);
            DrawRectangleRec(l10, YELLOW);
            DrawRectangleRec(l11, YELLOW);
            DrawRectangleRec(l12, YELLOW);

            DrawRectangleRec(r1, GREEN);
            DrawRectangleRec(r2, GREEN);
            DrawRectangleRec(r3, GREEN);
            DrawRectangleRec(r4, GREEN);
            DrawRectangleRec(r5, GREEN);
            DrawRectangleRec(r6, GREEN);
            DrawRectangleRec(r7, GREEN);
            DrawRectangleRec(r8, GREEN);
            DrawRectangleRec(r9, GREEN);
            DrawRectangleRec(r10, GREEN);
        }



        EndMode2D();



        std::ostringstream stream;
        switch (gameState) {
            case WAITING:
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, GetScreenHeight() / 5 * 3.5f, 20, WHITE);
                break;
            case FINISHED:
                DrawText(gameOverText.c_str(), GetScreenWidth() / 2 - gameOverTextWidth / 2, GetScreenHeight() / 3, 40, WHITE);
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, GetScreenHeight() / 5 * 3.5f, 20, WHITE);
                 //No break so it still renders score/timer for now
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
