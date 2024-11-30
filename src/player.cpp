#include "Object.h"
#include "raylib.h"

Object PlayerInit() {
    Texture2D sheet = LoadTexture("../sheet.png");

    Vector2 playerOffset{4,1};
    playerOffset.x = 4;
    playerOffset.y = 1;
    Object player(sheet, {16.0001, 0, 15.9999, 15.9999}, {25, 25, 16, 16}, {0, 0, 8, 15}, playerOffset);

    return player;
}

Object PlayerUpdates(Object player, float movementSpeed, float deltaTime) {
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
}