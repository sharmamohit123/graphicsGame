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

Ball ball[250];

Player player;

Ground ground[50];

Plank plank[100];

Pond pond, half_magnet;

Spike spike[20];

Trampoline tramp;

Magnet magnet;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, ran_r, ran_x, ran_y, angle, plank_x, plank_y, plank_l, plank_w, sq_y, p_x, p_y;
const float pi = 3.14159;
float top1, bottom1, left1=0, right1=0;

//float x1, x2, y2, x3, y3, slope, p_slope;

int i, n=200, m = 20, rand_ball, rotate, in_pond = 0, touch_pond = 0, ran_c, magnet_time=0;

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
        //if(ball[i].nplank != -1)
        ball[i].draw(VP);
    }
    for(i=0;i<25;i++){
        ground[i].draw(VP);
    }
    for(i=0;i<m;i++){
        plank[i].draw(VP);
    }
    pond.draw(VP);
    tramp.draw(VP);
    for(i=0;i<14;i++)
        spike[i].draw(VP);

    if(magnet_time >= 1000 && magnet_time <= 1500){
        magnet.position.x = screen_center_x - 7;
        magnet.position.y = screen_center_y + 6;
        half_magnet.position.x = screen_center_x - 7;
        half_magnet.position.y = screen_center_y + 6;
        magnet.draw(VP);
        half_magnet.draw(VP);
        //is_magnet = 1;
        player.magnet_force();
        if(magnet_time == 1500){
            magnet_time = 0;
            player.gravityspeedx = 0;
            //is_magnet = 0;
        }
    }
    player.draw(VP);

}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (left) {
        move_left();
    }
    if(right){
        move_right();
    }
    if(up){
        move_up();
    }
}

void tick_elements() {
    //ball1.tick();
    //ball2.tick();
    for(i=0;i<n;i++)
        ball[i].tick();
    player.tick(in_pond);
    for(i=0;i<n;i++){
        if (ball[i].nplank == -1 && detect_collision(player.bounding_box(), ball[i].bounding_box()) && player.gravityspeedy < 0) {
            player.rspeed = 0;
            player.jump(0);
            ball[i].position.x = -110;
            player.score += ball[i].kill_score;
            //ball2.speed = -ball2.speed;
        }
    }
    for(i=0;i<m;i++){
        plank[i].tick();
    }
    for(i=0;i<14;i++)
        spike[i].tick();

    if(player.score >= 2000 && player.score < 4000){
        if(player.level == 1){
            for(i=0;i<n;i++){
                ball[i].speed -= 0.1;
                if(ball[i].nplank!=-1)
                    plank[ball[i].nplank].speed -= 0.1;
            }
        }
        player.level=2;
    }
    else if(player.score >=4000){
        if(player.level == 2){
            for(i=0;i<n;i++){
                ball[i].speed -= 0.1;
                if(ball[i].nplank!=-1)
                    plank[ball[i].nplank].speed -= 0.1;
            }
        }
        player.level = 3;
    }

    if(player.lives == 0){
        quit(window);
    }
}

void refresh(){
    for(i=0;i<n;i++){
        if(ball[i].position.x - ball[i].radius > 100){
            ball[i].position.x = -100;
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
        ran_x = (rand() % 2000000 + 1 - 1000000)*0.0001;
        ran_y = (rand() % 90000 + 1 - 25000)*0.0001;
        ran_r = (rand() % 400 + 400)*0.001;
        ran_c =rand() % 7;
        ball[i] = Ball(ran_x, ran_y, ran_r, color[ran_c] );
        ball[i].speed = (rand() % 500 + 200)*(-0.0001);
        ball[i].kill_score = (ran_c+1)*5;
    }
        ground[0]=Ground(0, -1, COLOR_GREEN);
    for(i=1;i<25;i++)
        ground[i] = Ground(0, -i-1, COLOR_BROWN);
    pond = Pond(-1, -4, 2.5, COLOR_BLUE);
    tramp = Trampoline(6, -2.7, COLOR_DARK_RED);

    spike[0] = Spike( 10, -3.5, COLOR_BLACK);
    spike[1] = Spike( 10.4, -3.5, COLOR_BLACK);
    spike[2] = Spike( 10.8, -3.5, COLOR_BLACK);

    spike[3] = Spike( -6.5, -3.5, COLOR_BLACK);
    spike[4] = Spike( -6.9, -3.5, COLOR_BLACK);
    spike[5] = Spike( -7.3, -3.5, COLOR_BLACK);
    spike[6] = Spike( -7.7, -3.5, COLOR_BLACK);

    spike[7] = Spike( -20, -3.5, COLOR_BLACK);
    spike[8] = Spike( -20.4, -3.5, COLOR_BLACK);
    spike[9] = Spike( -20.8, -3.5, COLOR_BLACK);

    spike[10] = Spike( 40.5, -3.5, COLOR_BLACK);
    spike[11] = Spike( 40.9, -3.5, COLOR_BLACK);
    spike[12] = Spike( 41.3, -3.5, COLOR_BLACK);
    spike[13] = Spike( 41.7, -3.5, COLOR_BLACK);

    magnet = Magnet(-7, 6, COLOR_MAGNET);
    half_magnet = Pond(-7, 6, 0.5, COLOR_BACKGROUND);
    half_magnet.rotation = -90;

    for(i=0;i<m;i++){
        rand_ball = rand()%n;
        while(ball[rand_ball].nplank!=-1)
            rand_ball = rand()%n;
        angle = (rand()%30+10)*0.01;
        //rotate = rand()%360;
        plank_x = ball[rand_ball].position.x + ball[rand_ball].radius*cos(2*pi*angle);
        plank_y = ball[rand_ball].position.y + ball[rand_ball].radius*sin(2*pi*angle);
        plank_l = (rand()%2000+2000)*0.001;
        plank_w = (rand()%400+200)*0.001;
        printf("%f %f %d\n", plank_l, plank_w, rand_ball);
        ball[rand_ball].nplank = i;
        ball[rand_ball].angle = angle;
        /*if(i==0){
            ball[rand_ball].nplank = -1;
            plank[i] = Plank(4, 2, 4, 0.5, COLOR_BLACK);
            plank[i].rotation = 140;
            plank[i].speed = 0;
        }*/
        //else{
            plank[i] = Plank(plank_x, plank_y, plank_l, plank_w, COLOR_BLACK);
            plank[i].speed = ball[rand_ball].speed;

            plank[i].rotation = 360*angle - 90;
            plank[i].nball= rand_ball;
        //}
        //printf("length=%f width=%f\n", plank[i].length, plank[i].width);
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

            char score[50];
            sprintf(score,"%d",player.score);
            char str1[200]= "Score: ";
            strcat(str1,score);

            char lives[50];
            sprintf(lives,"%d",player.lives);
            //char str2[60]= "Lives: ";
            strcat(str1,"   Lives: ");
            strcat(str1, lives);

            char level[50];
            sprintf(level,"%d",player.level);
            //char str3[60]= "Level: ";
            strcat(str1,"   Level: ");
            strcat(str1, level);

            glfwSetWindowTitle(window,str1);

            tick_elements();
            refresh();
            tick_input(window);
            in_water();
            on_tramp();
            spike_check();
            spike_kill();
            touch_plank();
            span_screen();
            magnet_time+=1;
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
    if(player.gravityspeedy<0 && player.position.x>=4.7 && player.position.x <= 7.3 && player.position.y-player.radius<=-2.7){
        player.jump(1);
    }
}

void spike_check(){
    if(spike[0].position.x <= 1.7 || spike[2].position.x >= 60){
        for(i=0;i<3;i++)
            spike[i].speed = -spike[i].speed;
    }
    if(spike[3].position.x >=-4 || spike[6].position.x <= -50){
        for(i=3;i<7;i++)
            spike[i].speed = -spike[i].speed;
    }
    if(spike[10].position.x <= 1.7 || spike[13].position.x >= 60){
        for(i=10;i<14;i++)
            spike[i].speed = -spike[i].speed;
    }
    if(spike[7].position.x >=-4 || spike[9].position.x <= -50){
        for(i=7;i<10;i++)
            spike[i].speed = -spike[i].speed;
    }
}

void spike_kill(){
    if(player.position.y - player.radius <= -3 && player.gravityspeedy < 0){
        if((player.position.x +player.radius >= spike[0].position.x && player.position.x <= spike[2].position.x)
                || (player.position.x <= spike[3].position.x && player.position.x >= spike[6].position.x)
                || (player.position.x <= spike[7].position.x && player.position.x >= spike[9].position.x)
                || (player.position.x >= spike[10].position.x && player.position.x <= spike[13].position.x)){
            player.set_position(-5, -3.4);
            player.lives -= 1;
            //printf("lives=%d\n", player.lives);
        }
    }

}

void touch_plank(){
    float x1, x2, y1, y2, slope, m1, b, d;
    for(i=0;i<m;i++){
        plank_x = plank[i].position.x;
        plank_y = plank[i].position.y;
        plank_l = plank[i].length;
        plank_w = plank[i].width;
        slope = plank[i].rotation;
        x1 = player.position.x;
        y1 = player.position.y;
        m1 = tan(slope*pi/180);
        //printf("m1=%lf\n",m1);
        y2 = (m1*x1 - y1 - m1*plank_x + plank_y)/(1+m1*m1) + y1;
        x2 = (-1*m1)*(m1*x1 - y1 - m1*plank_x + plank_y)/(1+m1*m1) + x1;
        d = abs(m1*x1 - y1 - m1*plank_x + plank_y)/sqrt(1+m1*m1);
        b = (x2-plank_x)*(x2-plank_x)+(y2-plank_y)*(y2-plank_y);
        //printf("x2=%lf y2=%lf\n",x2,y2);
        if(sqrt(b)<= plank_l/2){
            if(d<player.radius && player.gravityspeedy<0){
                //player.jump(0);
                /*player.gravityspeedy = -1*((player.gravityspeedx+player.speedx)*sin(2*slope)-player.gravityspeedy*cos(2*slope))+0.3;
                if(slope>90)
                    player.rspeed = -0.5*((player.gravityspeedx+player.speedx)*cos(2*slope)+player.gravityspeedy*sin(2*slope));
                else
                    player.rspeed = 0.8*((player.gravityspeedx+player.speedx)*cos(2*slope)+player.gravityspeedy*sin(2*slope));*/
                player.gravityspeedy = -1*player.gravityspeedy;
                if(slope<=90 && slope>0)
                    player.rspeed = -0.2;
                else
                    player.rspeed = 0.2;

                ball[plank[i].nball].position.x=-110;
                plank[i].position.x = ball[plank[i].nball].position.x + ball[plank[i].nball].radius*cos(2*pi*ball[plank[i].nball].angle);
                player.score+=ball[plank[i].nball].kill_score;
            }
        }
    }
}

void span_screen(){
    screen_center_x = player.position.x + 4;
    screen_center_y = player.position.y*0.3 + 1;
    reset_screen();
}

void zoom_screen(int type){
    if(type == 1 && screen_zoom > 0.4)
        screen_zoom -= 0.03;
    else if(type == -1 && screen_zoom < 1.6){
        screen_zoom += 0.03;
    }
    //printf("zoom=%lf\n", screen_zoom);
}

void reset_screen() {
    top1    = screen_center_y + 8 / screen_zoom;
    bottom1 = screen_center_y - 8 / screen_zoom;
    left1   = screen_center_x - 8 / screen_zoom;
    right1  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left1, right1, bottom1, top1, 0.1f, 500.0f);
}
