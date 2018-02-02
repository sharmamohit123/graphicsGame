#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include "ball.h"


bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;
double press, px=-10, py=-10, x, y;
bool lbutton_down = false;
using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        // switch (key) {
        // case GLFW_KEY_C:
        // rectangle_rot_status = !rectangle_rot_status;
        // break;
        // case GLFW_KEY_P:
        // triangle_rot_status = !triangle_rot_status;
        // break;
        // case GLFW_KEY_X:
        //// do something ..
        // break;
        // default:
        // break;
        // }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
    switch (key) {
    case 'Q':
    case 'q':
        quit(window);
        break;
    default:
        break;
    }
}

/* Executed when a mouse button is pressed/released */

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action){
            lbutton_down = true;
        }
        else if(GLFW_RELEASE == action){
            lbutton_down = false;
        }
    }

    if(lbutton_down) {
         // do your drag here
        //move_left();
        glfwGetCursorPos(window, &x, &y);
        px = x;
        py = y;
        cursor_position_callback(window, px, py);
    }
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //glfwGetCursorPos(window, &x, &y);
    if(lbutton_down){
        if(xpos-px>0){
            move_right();
            //xpos = px;
        }
        else{
            move_left();
            //xpos = px;
        }
        px = xpos;
    }
    //printf("yoyo\n");
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // Do something
    if(yoffset<0)
        zoom_screen(1);
    if(yoffset>0)
        zoom_screen(-1);
}
