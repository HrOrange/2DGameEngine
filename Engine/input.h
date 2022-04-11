#pragma once
#include "common_includes.h"


//for keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//for mouse movement
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//for mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//for mouse button press and release
void mouse_button_press_callback(GLFWwindow* window, int button, int action, int mods);