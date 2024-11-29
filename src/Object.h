#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"

class Object {
private:
    Rectangle textureArea;
    Rectangle object;
    Rectangle hitbox;
public:
    Object(const Rectangle textureArea, const Rectangle object, const Rectangle hitbox) : textureArea(textureArea), object(object), hitbox(hitbox) {
        this->hitbox.x = getX() + 4;
        this->hitbox.y = getY() + 1;
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
        this->hitbox.x = x + 4;
    }

    float getY() const {
        return this->object.y;
    }

    void setY(const float y) {
        this->object.y = y;
        this->hitbox.y = y + 1;
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

    Rectangle getHitbox() {
        return this->hitbox;
    }
};

#endif