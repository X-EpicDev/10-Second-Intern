#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Machine.h"
#include "Object.h"
#include "Player.h"
#include "Printer.h"
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
    InitWindow(windowWidth, windowHeight, "Resize Window");
    SetWindowMinSize(320, 240);

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Load textures
    Texture2D sheet = LoadTexture("../assets/sheet.png");
    Texture2D overlayWall = LoadTexture("../assets/overlay_layer.png");
    Texture2D officeWalls = LoadTexture("../assets/collision_layer.png");
    Texture2D officeFloor = LoadTexture("../assets/floor_layer.png");

    // Text
    std::string startText = "Press SPACE or ENTER to begin";
    int startTextWidth = MeasureText(startText.c_str(), 20);

    std::string gameOverText = "Game Over";
    int gameOverTextWidth = MeasureText(gameOverText.c_str(), 40);

    // Tasks
    tasks.emplace_back(std::list<Types>{Types::PRINTER, Types::PRINTER}); // Do not list more than exists on the map or death happens
    tasks.emplace_back(std::list<Types>{Types::MACHINE});

    // Objects
    Player player(sheet, {16.0001, 32.0001, 15.9999, 31.9999}, {32, 32, 16, 32}, {0, 0, 8, 16}, Vector2{4, 15}, {0, 0, 10, 16}, {3, 10});
    Object overlay(overlayWall, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Object walls(officeWalls, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Object floor(officeFloor, {0, 0, 432, 384}, {0, 0, 432, 384}, {0, 0, 0, 0}, Vector2{0, 0});
    Image officeWallsImage = LoadImage("../assets/collision_layer.png");
    Color* wallPixels = LoadImageColors(officeWallsImage);

    std::vector<Object> printers = {
        Printer{sheet, Rectangle{0, 0, 32.9999, 15.9999}, Rectangle{8, 8 + 16, 16, 16}, Rectangle{0, 0, 8, 12}, Vector2{4, 4}},
        Printer{sheet, Rectangle{0, 0, 15.9999, 15.9999}, Rectangle{8 + 16 * 3, 8 + 16, 16, 16}, Rectangle{0, 0, 8, 12}, Vector2{4, 4}},
        Printer{sheet, Rectangle{0, 0, 15.9999, 15.9999}, Rectangle{8, 8 + 16 * 3, 16, 16}, Rectangle{0, 0, 8, 12}, Vector2{4, 4}},
        Printer{sheet, Rectangle{0, 0, 15.9999, 15.9999}, Rectangle{8 + 16 * 3, 8 + 16 * 3, 16, 16}, Rectangle{0, 0, 8, 12}, Vector2{4, 4}},
    };
    objects.emplace(Types::PRINTER, printers);

    std::vector<Object> machines = {
        Machine{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 8, 8 + 16 * 4, 16, 32}, Rectangle{0, 0, 8, 4}, Vector2{4, 12}},
        Machine{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 8, 8 + 16, 16, 32}, Rectangle{0, 0, 8, 4}, Vector2{4, 12}},
        Machine{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8, 8 + 16 * 8, 16, 32}, Rectangle{0, 0, 8, 4}, Vector2{4, 12}},
        Machine{sheet, Rectangle{0, 32.0001, 15.9999, 31.9999}, Rectangle{8 + 16 * 8, 8 + 16 * 8, 16, 32}, Rectangle{0, 0, 8, 4}, Vector2{4, 12}},
    };
    objects.emplace(Types::MACHINE, machines);

    // std::vector<Object> coffee = {
    //     Object{sheet, Rectangle{0, 16.0001, 15.9999, 15.9999}, Rectangle{8 + 16 * 7, 8 + 16 * 2, 16, 16}, Rectangle{0, 0, 12, 6}, Vector2{2, 8}},
    // };
    // objects.emplace(Types::COFFEE, coffee);

    Camera2D camera;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();
        //camera.rotation += (deltaTime * 3);

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

                if (IsKeyPressed(KEY_Q)) {
                    timer = 10;
                }
                if (IsKeyPressed(KEY_E) && player.currentObject != nullptr && currentTask->getNextObject() == player.currentObject) {
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

        camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.target = (Vector2){player.getX() + player.getWidth() / 2, player.getY() + player.getHeight() / 2};

        Vector2 mouse = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawCircle(100, 75, 12.5, RED);

        floor.draw(debug);

        for (auto& objectKey : objects) {
            for (Object& object : objectKey.second) {
                object.draw(debug);
            }
        }

        player.draw(debug);
        walls.draw(debug);
        overlay.draw(debug);

        if (player.currentObject != nullptr && currentTask != nullptr && currentTask->getNextObject() == player.currentObject) {
            DrawText("Press 'E' to interact", player.currentObject->getX() + 4, player.currentObject->getY() - 16, 1, WHITE);
        }

        { // TODO: AAAA TRIANGLE
            Vector2 targetPosition = { 600, 400 };

            float triangleSize = 30.0f;
            Vector2 trianglePosition = { player.getX(), player.getY() - 50 };

            Vector2 direction = { targetPosition.x - player.getX(), targetPosition.y - player.getY() };
            float angle = atan2(direction.y, direction.x); // Calculate the angle to the target

            // Define the triangle points (relative to the player)
            Vector2 point1 = { trianglePosition.x - triangleSize / 2, trianglePosition.y + triangleSize / 2 };
            Vector2 point2 = { trianglePosition.x + triangleSize / 2, trianglePosition.y + triangleSize / 2 };
            Vector2 point3 = { trianglePosition.x, trianglePosition.y - triangleSize };

            // Apply rotation to the triangle points based on the angle
            Matrix rotationMatrix = MatrixRotate({0,0,0}, angle);  // Create a rotation matrix
            point1 = Vector2Transform(point1, rotationMatrix);
            point2 = Vector2Transform(point2, rotationMatrix);
            point3 = Vector2Transform(point3, rotationMatrix);

            // Adjust the triangle's position based on the player
            point1 = Vector2Add(point1, {player.getX(), player.getY()});
            point2 = Vector2Add(point2, {player.getX(), player.getY()});
            point3 = Vector2Add(point3, {player.getX(), player.getY()});

            // DrawTriangle(point1, point2, point3, GREEN);
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
                if (currentTask == nullptr) {
                    stream.str("");
                    stream << std::fixed << std::setprecision(1) << std::round(cooldown * 10) / 10.0;
                    std::string text = "Cooldown: " + stream.str();
                    DrawText(text.c_str(), GetScreenWidth() / 2 - MeasureText(text.c_str(), 40) / 2, GetScreenHeight() / 5 , 40, WHITE);
                }
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

    UnloadImageColors(wallPixels);
    UnloadImage(officeWallsImage);
    UnloadTexture(officeWalls);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
