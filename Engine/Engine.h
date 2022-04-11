#pragma once
#include "camera.h"
#include "opengl_window.h"
#include "triangle.h"
#include "image.h"
#include "text_database.h"
#include "text.h"
#include "circle.h"
#include "rectangle.h"
#include "AudioManager.h"
#include "line.h"
#include "FPS_counter.h"
#include "gif.h"
#include "global_variables.h"


std::string getCurrentDir();
int init(opengl_window* window, std::string title, float background_color[4], uint8_t fullscreen = 0);
//this function is for clearing all the variables that have been saved on the heap and other things
int quit();

inline void set_input_callback(opengl_window* window, void (*func)(GLFWwindow*, int, int, int, int)){ glfwSetKeyCallback(window->m_window, func); }
inline void set_cursor_move_callback(opengl_window* window, void (*func)(GLFWwindow*, double, double)){ glfwSetCursorPosCallback(window->m_window, func); }
inline void set_scroll_callback(opengl_window* window, void (*func)(GLFWwindow*, double, double)) { glfwSetScrollCallback(window->m_window, func); }
inline void set_mouse_button_press_callback(opengl_window* window, void (*func)(GLFWwindow*, int, int, int)) { glfwSetMouseButtonCallback(window->m_window, func); }

//global objects functions
image* add_image(int x, int y, std::string file_name, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(int x, int y, std::string file_name, mobility chosen_mobility_for_image = mobility::camera_dependent);

image* add_image(std::vector<int> x, std::vector<int> y, std::string file_name, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(std::vector<int> x, std::vector<int> y, std::string file_name, mobility chosen_mobility_for_image = mobility::camera_dependent);

image* add_image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility_for_image = mobility::camera_dependent);


image* add_image(std::string array_name, int x, int y, std::string file_name, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(std::string array_name, int x, int y, std::string file_name, mobility chosen_mobility_for_image = mobility::camera_dependent);

image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::string file_name, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::string file_name, mobility chosen_mobility_for_image = mobility::camera_dependent);

image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, int index, mobility chosen_mobility_for_image = mobility::camera_dependent);
image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility_for_image = mobility::camera_dependent);