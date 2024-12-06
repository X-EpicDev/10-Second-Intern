#ifndef PRINTER_H
#define PRINTER_H

#include "Object.h"

class Printer : public Object {
private:

public:
    Printer(Texture2D& sheet, Rectangle textureArea, Rectangle object, Rectangle hitbox, Vector2 hitboxOffset) : Object(sheet, textureArea, object, hitbox, hitboxOffset) {}

    void interact() override {

    }
};

#endif //PRINTER_H
