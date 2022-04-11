#include "input.h"
#include "global_variables.h"
#include <Windows.h>

//for keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_D) right = true; //cam.move(-1.0f, 0.0f);
        else if (key == GLFW_KEY_A) left = true; //cam.move(1.0f, 0.0f);
        else if (key == GLFW_KEY_W) up = true; //cam.move(0.0f, -1.0f);
        else if (key == GLFW_KEY_S) down = true; //cam.move(0.0f, 1.0f);
        else if (key == GLFW_KEY_SPACE) {
            cam.set_position(0, 0, false);
            //fullscreen_mode = true;
        }
    }
    else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_D) right = false; //cam.move(-1.0f, 0.0f);
        else if (key == GLFW_KEY_A) left = false; //cam.move(1.0f, 0.0f);
        else if (key == GLFW_KEY_W) up = false; //cam.move(0.0f, -1.0f);
        else if (key == GLFW_KEY_S) down = false; //cam.move(0.0f, 1.0f);
    }
}
//for mouse movement
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MOUSE_X = (int)xpos;
    MOUSE_Y = (int)ypos;
    MOUSE_T_X = (MOUSE_X / (float)SCREEN_WIDTH - 0.5f) * 2.0f;
    MOUSE_T_Y = (MOUSE_Y / (float)SCREEN_HEIGHT - 0.5f) * -2.0f;
    //cam.zoom(0.0f);
}
//for mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.zoom((float)yoffset);
    //println(cam.m_camera_zoom);
}

void mouse_button_press_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == 0) {
        if (action) {
            MOUSE_LEFT_PRESS = 1;
            MOUSE_LEFT_LAST_PRESS_X = MOUSE_X;
            MOUSE_LEFT_LAST_PRESS_Y = MOUSE_Y;

            
            float mouse_tile_addition = (MOUSE_T_X + 1.0f) / 2.0f;
            float map_movement_tile_addition = -cam.m_t_camera_offset[0] / 2.0f * cam.m_camera_zoom;
            float zoom_border_x_addition = (SCREEN_WIDTH - floor(SCREEN_WIDTH / cam.m_reciprocal_zoom)) / 2.0f / SCREEN_WIDTH;
            println("-------");
            
            int tile_x = floor((mouse_tile_addition + map_movement_tile_addition - zoom_border_x_addition) * SCREEN_WIDTH * cam.m_reciprocal_zoom / 128.0f);
            println(tile_x);
        }
        else MOUSE_LEFT_PRESS = 0;
    }
    else if (button == 1) {
        if (action) {
            MOUSE_RIGHT_PRESS = 1;
            MOUSE_RIGHT_LAST_PRESS_X = MOUSE_X;
            MOUSE_RIGHT_LAST_PRESS_Y = MOUSE_Y;
        }
        else MOUSE_RIGHT_PRESS = 0;
    }
    else if (button == 2) {
        if (action) {
            MOUSE_MIDDLE_PRESS = 1;
            MOUSE_MIDDLE_LAST_PRESS_X = MOUSE_X;
            MOUSE_MIDDLE_LAST_PRESS_Y = MOUSE_Y;
        }
        else MOUSE_MIDDLE_PRESS = 0;
    }
}