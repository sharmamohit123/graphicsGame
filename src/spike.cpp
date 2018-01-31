#include "spike.h"
#include "main.h"

Spike::Spike(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->radius = r;
    this->rotation = 0;
    speed = 0.03;
    static const GLfloat vertex_buffer_data[] = {
        0, 0.5, 0, // vertex 1
        -0.2,  -0.5, 0, // vertex 2
        0.2,  -0.5, 0 // vertex 3

    };

    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color, GL_FILL);
}

void Spike::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Spike::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Spike::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Spike::bounding_box() {
    float x = this->position.x, y = this->position.y;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, 16, 2 };
    return bbox;
}
