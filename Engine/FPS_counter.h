#pragma once
#include "common_includes.h"
#include "text.h"

class FPS_counter {
public:
    int m_current_FPS;

    FPS_counter(uint8_t debug_level);
    FPS_counter(uint8_t debug_level, float color[4]); //0 = no debug; 1 = text in corner of screen; 2 = text in corner of screen and print in console
    ~FPS_counter();
    void update();
    void draw();
    void set_debug_level(int value);

private:
    //this is used to calculate FPS
    std::chrono::system_clock::time_point m_start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point m_now = std::chrono::system_clock::now();
    std::chrono::duration<double> m_delta_beginning = std::chrono::system_clock::now() - m_start;
    
    int m_counted_frames;
    double m_current_sec;
    uint8_t m_debug_level;
    text* m_text_renderer;
};