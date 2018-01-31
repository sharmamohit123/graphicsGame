#include "plank.h"
#include "main.h"

Plank::Plank(float x, float y, float l, float w, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->length = l;
    this->width = w;
    speed = 0.01;
    GLfloat vertex_buffer_data[] = {
        -this->length/2, this->width, 0, // vertex 1
        -this->length/2,  0, 0, // vertex 2
        this->length/2,  0, 0, // vertex 3

        this->length/2,  0, 0, // vertex 3
        this->length/2, this->width, 0, // vertex 4
        -this->length/2, this->width, 0 // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Plank::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plank::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plank::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Plank::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float l = length, w = width;
    bounding_box_t bbox = { x, y, l, w };
    return bbox;
}
