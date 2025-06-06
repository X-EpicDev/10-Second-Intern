#include <cmath>
#include <complex>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Object.h"
#include "Player.h"
#include "raylib.h"
#include "raymath.h"
#include "Task.h"

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
    float cooldown = 2.5f;
    int score = 0;
    Task* currentTask = nullptr;

    std::vector<Task> tasks;
    std::unordered_map<Types, std::vector<Object>> objects;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "10 Second Intern");
    SetWindowMinSize(320, 240);

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Load textures
    Texture2D sheet = LoadTexture("assets/sheet.png");
    Texture2D overlayWall = LoadTexture("assets/overlay_layer.png");
    Texture2D officeWalls = LoadTexture("assets/collision_layer.png");
    Texture2D officeFloor = LoadTexture("assets/floor_layer.png");
    Texture2D tableImage = LoadTexture("assets/table_layer.png");
    Texture2D startImage = LoadTexture("assets/start.png");

    // Text
    std::string startText = "Press SPACE or ENTER to begin";
    int startTextWidth = MeasureText(startText.c_str(), 20);

    std::string gameOverText = "Game Over";
    int gameOverTextWidth = MeasureText(gameOverText.c_str(), 40);

    std::string cooldownText = "Waiting for next task...";
    int cooldownWidth = MeasureText(cooldownText.c_str(), 40);


    // Tasks
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::PRINTER}); // Do not list more than exists on the map or death happens
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::PRINTER, Types::PRINTER});
    tasks.emplace_back(std::list<Types>{Types::PRINTER});
    tasks.emplace_back(std::list<Types>{Types::MACHINE, Types::PRINTER});
    tasks.emplace_back(std::list<Types>{Types::MACHINE});
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::MACHINE});
    tasks.emplace_back(std::list<Types>{Types::WATER, Types::STINKY});
    tasks.emplace_back(std::list<Types>{Types::WATER, Types::STINKY, Types::WATER});
    tasks.emplace_back(std::list<Types>{Types::COFFEE, Types::STINKY});
    tasks.emplace_back(std::list<Types>{Types::COFFEE, Types::STINKY, Types::STINKY});
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::COFFEE});
    tasks.emplace_back(std::list<Types>{Types::MACHINE, Types::COFFEE});
    tasks.emplace_back(std::list<Types>{Types::STINKY, Types::COFFEE});
    tasks.emplace_back(std::list<Types>{Types::COFFEE, Types::PRINTER, Types::PRINTER});
    tasks.emplace_back(std::list<Types>{Types::WATER, Types::MACHINE, Types::COFFEE});
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::MACHINE, Types::PRINTER});

    // Objects
    Player player(sheet, {16.0001, 32.0001, 15.9999, 31.9999}, {32, 32, 16, 32}, {0, 0, 8, 9}, Vector2{4, 22}, {0, 0, 10, 16}, {3, 10});
    Object overlay(overlayWall, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Object tableLayer(tableImage, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Object walls(officeWalls, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Object floor(officeFloor, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Image officeWallsImage = LoadImage("assets/collision_layer.png");
    Image appIcon = LoadImage("assets/icon.png");
    Color* wallPixels = LoadImageColors(officeWallsImage);

    SetWindowIcon(appIcon);

    std::vector<Object> printers = {
        Object{sheet, Rectangle{32.0001, 16.0001, 15.9999, 15.9999}, Rectangle{16 * 3, 16 + 16, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 16.0001, 15.9999, 15.9999}, Rectangle{16 * 5, 16 + 16 * 4, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 16.0001, 15.9999, 15.9999}, Rectangle{16 * 19, 16, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 16.0001, 15.9999, 15.9999}, Rectangle{16 * 23, 16 * 12, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 16.0001, 15.9999, 15.9999}, Rectangle{16 * 11, 16 * 11, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
    };
    objects.emplace(Types::PRINTER, printers);

    std::vector<Object> machines = {
        Object{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 12, 16 - 8, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
        Object{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16, 16 * 17, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
        Object{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 18, 16 * 12, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
    };
    objects.emplace(Types::MACHINE, machines);

    std::vector<Object> coffee = {
        Object{sheet, Rectangle{32.0001, 0, 15.9999, 15.9999}, Rectangle{16 * 5, 16 * 2, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 0, 15.9999, 15.9999}, Rectangle{8 + 16 * 20, 16, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 0, 15.9999, 15.9999}, Rectangle{8 + 16 * 24, 16 * 12, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
        Object{sheet, Rectangle{32.0001, 0, 15.9999, 15.9999}, Rectangle{8 + 16 * 4, 16 * 17, 16, 16}, Rectangle{0, 0, 24, 10}, Vector2{-5, 10}},
    };
    objects.emplace(Types::COFFEE, coffee);

    std::vector<Object> water = {
        Object{sheet, Rectangle{32.0001, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 11, 16 * 17, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
        Object{sheet, Rectangle{32.0001, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 19, 16 * 17, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
        Object{sheet, Rectangle{32.0001, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 19, 16 * 12, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
    };
    objects.emplace(Types::WATER, water);

    std::vector<Object> stinky = {
        Object{sheet, Rectangle{48.0001, 0.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 17, 16 * 17, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
        Object{sheet, Rectangle{48.0001, 0.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 18, 16 * 17, 16, 32}, Rectangle{0, 0, 8, 10}, Vector2{4, 16}},
    };
    objects.emplace(Types::STINKY, stinky);

    std::vector<Object> tables = {
        Object{sheet, Rectangle{0, 0, 0, 0}, Rectangle{8 + 16 * 2, 16 * 5, 32, 16}, Rectangle{0, 0, 32, 10}, Vector2{0, 13}},
        Object{sheet, Rectangle{0, 0, 0, 0}, Rectangle{8 + 16 * 2, 16 * 8, 32, 16}, Rectangle{0, 0, 32, 10}, Vector2{0, 13}},
        Object{sheet, Rectangle{0, 0, 0, 0}, Rectangle{8 + 16 * 4, 16 * 8, 32, 16}, Rectangle{0, 0, 32, 10}, Vector2{0, 13}},
        Object{sheet, Rectangle{0, 0, 0, 0}, Rectangle{8 + 16 * 12, 16 * 11, 32, 16}, Rectangle{0, 0, 32, 10}, Vector2{0, 13}},
    };
    objects.emplace(Types::BLANK_TABLE, tables);

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    float lastX = 0;
    float lastY = 0;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_F3)) {
            debug = !debug;
        }

        // Update
        bool found = false;
        Vector2 beforePos;
        switch (gameState) {
            case WAITING:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    gameState = PLAYING;
                    currentTask = &tasks.at(std::rand() % tasks.size());
                    currentTask->start();
                    currentTask->pickRandom(objects);
                }
                break;
            case PLAYING:
                if (currentTask == nullptr) {
                    cooldown -= deltaTime;
                    if (cooldown <= 0) {
                        currentTask = &tasks.at(std::rand() % tasks.size());
                        currentTask->start();
                        currentTask->pickRandom(objects);
                        timer = 10;
                    }
                } else {
                    timer -= deltaTime;
                    if (timer <= 0) {
                        timer = 0;
                        gameState = FINISHED;
                        break;
                    }
                }

                beforePos.x = player.getX();
                beforePos.y = player.getY();
                player.input(deltaTime, movementSpeed, wallPixels, &officeWalls);

                for (auto& objectKey : objects) {
                    for (Object& object : objectKey.second) {
                        if (CheckCollisionRecs(object.getHitbox(), player.getHitbox())) {
                            player.setY(beforePos.y);
                        }
                        if (CheckCollisionRecs(object.getHitbox(), player.getHitbox())) {
                            player.setX(beforePos.x);
                        }

                        if (CheckCollisionRecs(object.getHitbox(), player.getInteractionHitbox())) {
                            player.currentObject = &object;
                            found = true;
                            goto OUTSIDE_OBJECTS;
                        }
                    }
                }
                OUTSIDE_OBJECTS:

                if (!found) {
                    player.currentObject = nullptr;
                }

                if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        && player.currentObject != nullptr && currentTask != nullptr && currentTask->getNextObject() == player.currentObject) {
                    player.currentObject->interact();
                    if (currentTask->pickRandom(objects)) {
                        score++;
                        currentTask = nullptr;
                        cooldown = 1;
                    }
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

        camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        if (abs(player.getX() - (camera.target.x - 20)) > 15) {
            camera.target.x += player.getX() - lastX;
        }
        if (abs(player.getY() - (camera.target.y - 20)) > 15) {
            camera.target.y += player.getY() - lastY;
        }
        lastX = player.getX();
        lastY = player.getY();

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawCircle(100, 75, 12.5, RED);

        floor.draw(debug);
        walls.draw(debug);
        tableLayer.draw(debug);

        for (auto& objectKey : objects) {
            for (Object& object : objectKey.second) {
                object.draw(debug);
            }
        }

        player.draw(debug);
        overlay.draw(debug);

        if (player.currentObject != nullptr && currentTask != nullptr && currentTask->getNextObject() == player.currentObject) {
            DrawText("Press 'E' or click", player.currentObject->getX() + 4, player.currentObject->getY() - 16, 1, WHITE);
        }

        if(currentTask != nullptr) {
            Vector2 targetPosition = {currentTask->getNextObject()->getX() + (currentTask->getNextObject()->getWidth() / 2), currentTask->getNextObject()->getY() + (currentTask->getNextObject()->getHeight() / 2)};  // Target position
            DrawLineEx({player.getX() + (player.getWidth() / 2), player.getY() + (player.getHeight() / 4)}, targetPosition,5, RED);
        }

        EndMode2D();

        float scaleX = (float)GetScreenWidth() / (float)startImage.width;
        float scaleY = (float)GetScreenHeight() / (float)startImage.height;

        // Use the smaller scaling factor to maintain aspect ratio
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        // Calculate the new dimensions based on the scale factor
        int newWidth = (int)(startImage.width * scale);
        int newHeight = (int)(startImage.height * scale);

        // Calculate position to center the image
        float posX = (GetScreenWidth() - newWidth) / 2;
        float posY = (GetScreenHeight() - newHeight) / 2;

        std::ostringstream stream;
        std::string text;
        switch (gameState) {
            case WAITING:
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, static_cast<int>(static_cast<float>(GetScreenHeight()) / 5.0f * 3.5f), 20, WHITE);

                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 191, 255, 255});
                DrawTextureEx(startImage, Vector2{ posX, posY }, 0.0f, scale, WHITE);
                break;
            case FINISHED:
                DrawText(gameOverText.c_str(), GetScreenWidth() / 2 - gameOverTextWidth / 2, GetScreenHeight() / 3, 40, WHITE);
                DrawText(startText.c_str(), GetScreenWidth() / 2 - startTextWidth / 2, static_cast<int>(static_cast<float>(GetScreenHeight()) / 5.0f * 3.5f), 20, WHITE);

                DrawText(("Score: " + std::to_string(score)).c_str(), GetScreenWidth() / 2 - MeasureText(("Score: " + std::to_string(score)).c_str(), 30), GetScreenHeight() / 3 + 48, 30, WHITE);
                break;
            case PLAYING:
                DrawText(("Score: " + std::to_string(score)).c_str(), 0, 0, 20, WHITE);
                // Format to 1 decimal place
                stream << std::fixed << std::setprecision(1) << std::round(timer * 10) / 10.0;
                DrawText(("Time: " + stream.str()).c_str(), 0, 16, 20, WHITE);
                if (currentTask == nullptr) {
                    DrawText(cooldownText.c_str(), GetScreenWidth() / 2 - cooldownWidth / 2, GetScreenHeight() / 5 , 40, WHITE);
                }
                break;
        }

        if (debug) {
            DrawFPS(0, 32);
            DrawText(("Position: " + std::to_string(player.getX()) + "," + std::to_string(player.getY())).c_str(), 0, 48, 20, WHITE);
            DrawText(("DeltaTime: " + std::to_string(deltaTime)).c_str(), 0, 64, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadImageColors(wallPixels);
    UnloadImage(officeWallsImage);
    UnloadTexture(officeWalls);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
