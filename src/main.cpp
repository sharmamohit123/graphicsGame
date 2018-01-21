#include "main.h"
#include "stdlib.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "pond.h"
#include "plank.h"
#include "ground.h"
#include "math.h"
#include "spike.h"
#include "trampoline.h"
#include "magnet.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball[100];

Player player;

Ground ground[5];

Plank plank[100];

Pond pond, half_magnet;

Spike spike[20];

Trampoline tramp;

Magnet magnet;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, ran_r, ran_x, ran_y, angle, plank_x, plank_y, plank_l, plank_w, sq_y, p_x, p_y;
const float pi = 3.14159;

int i, n=20, m=3, rand_ball, rotate, in_pond = 0, touch_pond = 0, ran_c;

color_t color[] = {COLOR_BALL1, COLOR_BALL2, COLOR_BALL3, COLOR_BALL4, COLOR_BALL5, COLOR_BALL6, COLOR_BALL7};

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    //ball1.draw(VP);
    //ball2.draw(VP);

    for(i=0;i<n;i++){
        ball[i].draw(VP);
    }
    for(i=0;i<4;i++){
        ground[i].draw(VP);
    }
    for(i=0;i<m;i++){
        plank[i].draw(VP);
    }
    pond.draw(VP);
    tramp.draw(VP);
    for(i=0;i<7;i++)
        spike[i].draw(VP);
    magnet.draw(VP);
    half_magnet.draw(VP);
    player.draw(VP);

}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (left) {
        // Do something
    }
}

void tick_elements() {
    //ball1.tick();
    //ball2.tick();
    for(i=0;i<n;i++)
        ball[i].tick();
    player.tick(in_pond);
    for(i=0;i<n;i++){
        if (detect_collision(player.bounding_box(), ball[i].bounding_box()) && player.gravityspeed < 0) {
            player.jump(0);
            ball[i].position.x = -10;
            if(ball[i].nplank != -1){
                plank[ball[i].nplank].position.x = ball[i].position.x + ball[i].radius*cos(2*pi*ball[i].angle);
            }
            player.score += 10;
            //ball2.speed = -ball2.speed;
        }
    }
    for(i=0;i<m;i++){
        plank[i].tick();
    }
    for(i=0;i<7;i++)
        spike[i].tick();
}

void refresh(){
    for(i=0;i<n;i++){
        if(ball[i].position.x - ball[i].radius > 8){
            ball[i].position.x = -8;
            if(ball[i].nplank != -1){
                plank[ball[i].nplank].position.x = ball[i].position.x + ball[i].radius*cos(2*pi*ball[i].angle);
                //plank[ball[i].nplank].position.y = ball[i].position.y + ball[i].radius*sin(2*pi*angle);
            }
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    //int ran_x, ran_y, ran_r, i;

    player = Player(-5, -3.4, 0.6, COLOR_RED);
    for(i=0;i<n;i++){
        ran_x = (rand() % 160000 + 1 - 80000)*0.0001;
        ran_y = (rand() % 90000 + 1 - 25000)*0.0001;
        ran_r = (rand() % 400 + 400)*0.001;
        ran_c =rand() % 7;
        ball[i] = Ball(ran_x, ran_y, ran_r, color[ran_c] );
        ball[i].speed = (rand() % 500 + 200)*(-0.0001);
    }
        ground[0]=Ground(0, -1, COLOR_GREEN);
    for(i=1;i<4;i++)
        ground[i] = Ground(0, -i-1, COLOR_BROWN);
    pond = Pond(-1, -4, 2.5, COLOR_BLUE);
    tramp = Trampoline(6, -2.7, COLOR_DARK_RED);

    spike[0] = Spike( 2, -3.5, COLOR_BLACK);
    spike[1] = Spike( 2.4, -3.5, COLOR_BLACK);
    spike[2] = Spike( 2.8, -3.5, COLOR_BLACK);
    spike[3] = Spike( -6.5, -3.5, COLOR_BLACK);
    spike[4] = Spike( -6.9, -3.5, COLOR_BLACK);
    spike[5] = Spike( -7.3, -3.5, COLOR_BLACK);
    spike[6] = Spike( -7.7, -3.5, COLOR_BLACK);

    magnet = Magnet(-7, 6, COLOR_MAGNET);
    half_magnet = Pond(-7, 6, 0.5, COLOR_BACKGROUND);
    half_magnet.rotation = -90;

    for(i=0;i<m;i++){
        rand_ball = rand()%n;
        while(ball[rand_ball].nplank!=-1)
            rand_ball = rand()%n;
        angle = (rand()%50+1)*0.01;
        //rotate = rand()%360;
        plank_x = ball[rand_ball].position.x + ball[rand_ball].radius*cos(2*pi*angle);
        plank_y = ball[rand_ball].position.y + ball[rand_ball].radius*sin(2*pi*angle);
        plank_l = (rand()%2000+2000)*0.001;
        plank_w = (rand()%400+200)*0.001;
        printf("%f %f %d\n", plank_l, plank_w, rand_ball);
        ball[rand_ball].nplank = i;
        ball[rand_ball].angle = angle;
        plank[i] = Plank(plank_x, plank_y, plank_l, plank_w, COLOR_BLACK);
        plank[i].speed = ball[rand_ball].speed;
        plank[i].rotation = 360*angle - 90;
    }


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands

            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            refresh();
            tick_input(window);
            in_water();
            on_tramp();
            spike_check();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < 0.8) && ((a.y - b.y) * 2 < (a.height + b.height) && (a.y - b.y)>0);
    //return ((a.y - b.y) * 2 < (a.height + b.height));
}

void move_right(){
    //printf("speed = %lf", player.speedx);
    player.right(in_pond);
}

void move_left(){
    //printf("speed = %lf", player.speedxx);
    player.left(in_pond);
}

void move_up(){
    touch_pond = 0;
    if(player.position.y - player.radius <= -4)
        player.jump(0);
}

void in_water(){
    float left = pond.position.x - 2.5;
    float right = pond.position.x + 2.5;
    //printf("left=%f right%f",left, right);
    float p_radius = 2.5 - player.radius;
    if(player.position.x > left && player.position.x < right && player.position.y -player.radius <=-4 && in_pond==0){
        in_pond = 1;
        if(player.position.x < left+player.radius){
            player.position.x = -2.9;
            player.position.y = -4;
            touch_pond = 1;
        }
        if(player.position.x > right-player.radius){
            player.position.x = 0.9;
            player.position.y = -4;
            touch_pond = 1;
        }
    }
    if(in_pond && !touch_pond){
        player.position.y -= 0.03;
        p_x = pond.position.x - player.position.x;
        sq_y = p_radius*p_radius - p_x*p_x;
        p_y = pond.position.y - pow(sq_y, 0.5) ;
        if(player.position.y <= p_y){
            touch_pond = 1;
            player.position.y = p_y;
        }
    }
    if(in_pond && touch_pond && player.position.y - player.radius > pond.position.y-2.5){
        if(player.position.x < pond.position.x)
            player.position.x += 0.02;
        else
            player.position.x -= 0.02;
    }

    if(touch_pond){
        p_x = pond.position.x - player.position.x;
        sq_y = p_radius*p_radius - p_x*p_x;
        player.position.y = pond.position.y - pow(sq_y, 0.5) ;
    }

    if(in_pond){
        if(player.position.x <= left+player.radius){
            player.position.x = left;
            player.position.y = -4 + player.radius;
        }
        if(player.position.x >= right-player.radius){
            player.position.x = right;
            player.position.y = -4 + player.radius;
        }
    }
    if(player.position.x >= right || player.position.x <= left || player.position.y-player.radius>-4){
        in_pond = 0;
        touch_pond = 0;
    }
}

void on_tramp(){
    if(player.gravityspeed<0 && player.position.x>=4.7 && player.position.x <= 7.3 && player.position.y-player.radius<=-2.7){
        player.jump(1);
    }
}

void spike_check(){
    if(spike[0].position.x <= 1.7 || spike[2].position.x >= 7.8){
        for(i=0;i<3;i++)
            spike[i].speed = -spike[i].speed;
    }
    if(spike[3].position.x >=-3.7 || spike[6].position.x <= -7.8){
        for(i=3;i<7;i++)
            spike[i].speed = -spike[i].speed;
    }
}

void reset_screen() {
    float top    = screen_center_y + 8 / screen_zoom;
    float bottom = screen_center_y - 8 / screen_zoom;
    float left   = screen_center_x - 8 / screen_zoom;
    float right  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
