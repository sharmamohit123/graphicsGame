#include "main.h"

#ifndef Magnet_H
#define Magnet_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    //float radius;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    //void tick();
    //double speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Magnet_H
