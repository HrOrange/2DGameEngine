#pragma once
#include "common_includes.h"

class opengl_window {
public:
    std::vector<void*> m_objects;
    GLFWwindow* m_window;
    unsigned int m_width, m_height;
    std::string PATH;

    opengl_window();
    opengl_window(int width, int height);
    opengl_window(int width, int height, bool fullscreen_setting);
    opengl_window(bool fullscreen_setting);
    opengl_window(int width, int height, const std::string& title, bool fullscreen_setting, float background_color[4], bool visible = true, GLFWwindow* share_resources_with_window = NULL);
    ~opengl_window();

    //fullscreen_setting: 0 = window, 1 = fullscreen, 2 = bordless fullscreen
    void init(int width, int height, std::string title, uint8_t fullscreen_setting, float background_color[4], bool visible = true, GLFWwindow* share_resources_with = NULL);
    void update();
    int add_triangle();
    int stay_open();
    void toggle_fullscreen();
    void close();

private:
    int m_triangles = -1;
    std::string m_title;
    float m_background_color[4];
    uint8_t m_fullscreen_setting;
};