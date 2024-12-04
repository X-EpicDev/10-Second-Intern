#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player : public Object {
private:
    static bool CheckCollisionWithTexture(const Rectangle rect, const Color* pixels, const int textureWidth, const int textureHeight) {
        for (int y = static_cast<int>(rect.y); y < rect.y + rect.height; y++) {
            for (int x = static_cast<int>(rect.x); x < rect.x + rect.width; x++) {
                if (x >= 0 && x < textureWidth && y >= 0 && y < textureHeight) {
                    int pixelIndex = y * textureWidth + x;
                    if (pixels[pixelIndex].a > 0) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static float ResolveAxisCollision(Rectangle* rect, const Color* pixels, const int textureWidth, const int textureHeight, const float delta, const char axis) {
        if (delta == 0.0f) return 0.0f;

        const float step = (delta > 0) ? 1.0f : -1.0f;
        float remaining = delta;

        while (remaining != 0.0f) {
            if (axis == 'x') {
                rect->x += step;
            } else if (axis == 'y') {
                rect->y += step;
            }

            if (CheckCollisionWithTexture(*rect, pixels, textureWidth, textureHeight)) {
                if (axis == 'x') {
                    rect->x -= step;
                } else if (axis == 'y') {
                    rect->y -= step;
                }
                break;
            }

            remaining -= step;
            if ((delta > 0 && remaining < 0) || (delta < 0 && remaining > 0)) {
                break;
            }
        }

        return delta - remaining;
    }

    Rectangle interactBox;
    Vector2 interactBoxOffset;

public:
    Object* currentObject{};

    Player(Texture2D& sheet, Rectangle textureArea, Rectangle object, Rectangle hitbox, Vector2 hitboxOffset, Rectangle interactBox, Vector2 interactBoxOffset) : Object(sheet, textureArea, object, hitbox, hitboxOffset), interactBox(interactBox), interactBoxOffset(interactBoxOffset) {}

    void input(float deltaTime, float movementSpeed, Color* wallPixels, Texture2D* officeWalls) {
        float dx = 0.0f;
        float dy = 0.0f;

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            dx += movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            dx -= movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            dy += movementSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            dy -= movementSpeed * deltaTime;
        }

        setX(getX() + ResolveAxisCollision(&getHitbox(), wallPixels, officeWalls->width, officeWalls->height, dx, 'x'));
        setY(getY() + ResolveAxisCollision(&getHitbox(), wallPixels, officeWalls->width, officeWalls->height, dy, 'y'));
    }

    void draw(bool debug) {
        if (debug) {
            DrawRectanglePro(this->interactBox, (Vector2){0, 0}, 0, BLUE);
        }
        Object::draw(debug);
    }

    void setX(const float x) {
        Object::setX(x);
        this->interactBox.x = x + this->interactBoxOffset.x;
    }

    void setY(const float y) {
        Object::setY(y);
        this->interactBox.y = y + this->interactBoxOffset.y;
    }

    Rectangle getInteractionHitbox() {
        return this->interactBox;
    }

    void interact() override {

    }
};

#endif
