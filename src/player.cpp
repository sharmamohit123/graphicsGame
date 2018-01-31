#include "player.h"
#include "main.h"

Player::Player(float x, float y, float r, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->radius = r;
    this->rotation = 0;
    speedx = 0.1;
    speedy = 0;
    gravity = -0.02;
    gravityspeedx = 0;
    gravityspeedy = 0;
    rspeed = 0;
    score = 0;
    level = 1;
    lives = 5;
    /*static const GLfloat vertex_buffer_data[] = {
        -0.2, -0.2, 0, // vertex 1
        0.2,  -0.2, 0, // vertex 2
        0.2,  0.2, 0, // vertex 3

        0.2,  0.2, 0, // vertex 3
        -0.2, 0.2, 0, // vertex 4
        -0.2, -0.2, 0 // vertex 1
    };*/
    int pos=0,i,j,n=96;
        //Any polygon can be created by changing of n
        GLfloat g_vertex_buffer_data[9*n];
        float pi = 3.14159, angle = 0, theta=(2*pi)/n;
        for(i=0;i<n;i++){
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            for(j=0;j<2;j++){
                g_vertex_buffer_data[pos++]= cos(angle)*this->radius;
                g_vertex_buffer_data[pos++]= sin(angle)*this->radius;
                g_vertex_buffer_data[pos++]= 0.0f;
                angle += theta;
            }
            angle-=theta;
        }

    this->object = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data, COLOR_BALL1, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data + 3*n/2, COLOR_BALL2, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data + 3*n, COLOR_BALL3, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data + 9*n/2, COLOR_BALL4, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data + 6*n, COLOR_BALL5, GL_FILL);
    this->object5 = create3DObject(GL_TRIANGLES, n/2, g_vertex_buffer_data + 15*n/2, COLOR_BALL6, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
    draw3DObject(this->object5);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick(int in_pond) {
    //this->position.x -= speedx;
    if(!in_pond){
        gravityspeedy += gravity;
        this->position.y += speedy+gravityspeedy;

        this->position.x += rspeed;

        if(this->position.y-this->radius <= -4){
            this->position.y = -4 + this->radius;
            speedy = 0;
            gravityspeedy = 0;
            gravity = 0;
            rspeed = 0;
        }
    }

    if(in_pond){
        gravityspeedy += gravity;
        this->position.y+=speedy+gravityspeedy*0.7;

    }
    if(this->position.x > 65)
        this->position.x = -65;
    if(this->position.x < -65)
        this->position.x = 65;
}

void Player::magnet_force(){
    //gravity = 0.0002;
    this->rotation += 10;
    gravityspeedx += 0.0002;
    //if(this->position.x - gravityspeedx - this->radius >= -8)
        this->position.x -= gravityspeedx;
    //printf("gravityspeedy  = %f", gravityspeedx);
}

void Player::right(int in_pond) {
    this->rotation -= 10;
    //if(this->position.x + speedx + this->radius <= 8){
        if(in_pond)
            this->position.x += speedx*0.7;
        else
            this->position.x += speedx;
    //}

}

void Player::left(int in_pond) {
    this->rotation += 10;
    //if(this->position.x - speedx - this->radius >= -8){
        if(in_pond)
            this->position.x -= speedx*0.7;
        else
            this->position.x -= speedx;
    //}
}

void Player::jump(int on_tramp){
    if(on_tramp)
        gravityspeedy = 0.6;
    else
        gravityspeedy = 0.4;

    gravity = -0.02;
}


bounding_box_t Player::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, 2*w, 2*h };
    return bbox;
}
