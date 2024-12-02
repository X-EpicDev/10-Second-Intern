#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"

class Object {
private:
    Texture2D& sheet;
    Rectangle textureArea;
    Rectangle object;
    Rectangle hitbox;
    Vector2 hitboxOffset;
public:
    Object(Texture2D& sheet, const Rectangle textureArea, const Rectangle object, const Rectangle hitbox, const Vector2 hitboxOffset) : sheet(sheet), textureArea(textureArea), object(object), hitbox(hitbox), hitboxOffset(hitboxOffset) {
        this->hitbox.x = getX() + this->hitboxOffset.x;
        this->hitbox.y = getY() + this->hitboxOffset.y;
    }

    void draw() {
        DrawRectanglePro(this->hitbox, (Vector2){0, 0}, 0, YELLOW);
        DrawTexturePro(sheet, this->textureArea, this->object, (Vector2){0, 0}, 0, WHITE);
    }

    Rectangle getTextureArea() {
        return this->textureArea;
    }

    Rectangle getRectangle() {
        return this->object;
    }

    float getX() const {
        return this->object.x;
    }

    void setX(const float x) {
        this->object.x = x;
        this->hitbox.x = x + this->hitboxOffset.x;
    }

    float getY() const {
        return this->object.y;
    }

    void setY(const float y) {
        this->object.y = y;
        this->hitbox.y = y + this->hitboxOffset.y;
    }

    float getWidth() const {
        return this->object.width;
    }

    void setWidth(const float width) {
        this->object.width = width;
    }

    float getHeight() const {
        return this->object.height;
    }

    void setHeight(const float height) {
        this->object.height = height;
    }

    Rectangle& getHitbox() {
        return this->hitbox;
    }
};

#endif