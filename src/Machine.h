#ifndef MACHINE_H
#define MACHINE_H

#include "Object.h"

class Machine : public Object {
private:

public:
    Machine(Texture2D& sheet, Rectangle textureArea, Rectangle object, Rectangle hitbox, Vector2 hitboxOffset) : Object(sheet, textureArea, object, hitbox, hitboxOffset) {}

    void interact() override {

    }
};

#endif //MACHINE_H
