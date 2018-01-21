#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->radius = r;
    this->rotation = 0;
    //speed = 0.01;
    int pos=0,i,j,n=100;
        //Any polygon can be created by changing of n
        GLfloat g_vertex_buffer_data[9*n];
        GLfloat l_vertex_buffer_data[9*n+36];
        float pi = 3.14159, angle = pi/2, theta=(2*pi)/(2*n);
        for(i=0;i<n;i++){
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            for(j=0;j<2;j++){
                g_vertex_buffer_data[pos++]= cos(angle)*1;
                g_vertex_buffer_data[pos++]= sin(angle)*1;
                g_vertex_buffer_data[pos++]= 0.0f;
                angle += theta;
            }
            angle-=theta;
        }
        static const GLfloat vertex_buffer_data[] = {
            0, -0.5, 0, // vertex 1
            1,  -0.5, 0, // vertex 2
            0,  -1, 0, // vertex 3

            0,  -1, 0, // vertex 3
            1, -1, 0, // vertex 4
            1, -0.5, 0,  // vertex 1

            0, 0.5, 0, // vertex 1
            1,  0.5, 0, // vertex 2
            1,  1, 0, // vertex 3

            0,  1, 0, // vertex 3
            1, 1, 0, // vertex 4
            0, 0.5, 0  // vertex 1

        };

        for(i=0;i<9*n+36;i++){
            if(i<9*n)
                l_vertex_buffer_data[i] = g_vertex_buffer_data[i];
            else
                l_vertex_buffer_data[i] = vertex_buffer_data[i-9*n];
        }

    this->object = create3DObject(GL_TRIANGLES, 3*n+12, l_vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

/*void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}*/

bounding_box_t Magnet::bounding_box() {
    float x = this->position.x, y = this->position.y;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, 16, 2 };
    return bbox;
}
