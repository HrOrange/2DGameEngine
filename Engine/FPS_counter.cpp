#include "FPS_counter.h"
#include "global_variables.h"

FPS_counter::FPS_counter(uint8_t debug_level)
	: m_counted_frames(0),
	  m_current_sec(0),
	  m_current_FPS(0),
      m_debug_level(debug_level)
{
    //text_database font_generator("arial", 35);

    float color[4] = { 255.0f, 50.0f, 50.0f, 255.0f };
    m_text_renderer = new text(FONTS["arial"], "FPS", 10.0f, 10.0f, color);
}
FPS_counter::FPS_counter(uint8_t debug_level, float color[4])
    : m_counted_frames(0),
    m_current_sec(0),
    m_current_FPS(0),
    m_debug_level(debug_level)
{
    //text_database font_generator("arial", 35);

    m_text_renderer = new text(FONTS["arial"], "FPS", 10.0f, 10.0f, color);
}

void FPS_counter::set_debug_level(int value){

    m_debug_level = value;
}

void FPS_counter::update() {
    
    delta_time = (float)(std::chrono::system_clock::now() - m_now).count() / 10000000.0f;
    total_time += delta_time;

    m_counted_frames++;
    m_now = std::chrono::system_clock::now();
    m_delta_beginning = m_now - m_start;
    if (floor(m_delta_beginning.count()) > m_current_sec) {
        m_current_sec = floor(m_delta_beginning.count());
        m_current_FPS = m_counted_frames;
        m_counted_frames = 0;

        
        if (m_debug_level == 1) {
            m_text_renderer->set_text("[FPS: " + std::to_string(m_current_FPS) + "]");
        }
        else if (m_debug_level == 2) {
            m_text_renderer->set_text("[FPS: " + std::to_string(m_current_FPS) + "]");
            println("FPS: " + std::to_string(m_current_FPS));
        }
        //my_text.set_text("[FPS: " + std::to_string(current_FPS) + "]");
    }

    if (m_debug_level > 0) {
        m_text_renderer->draw();
    }

}
void FPS_counter::draw() {

}

FPS_counter::~FPS_counter(){
    if (m_debug_level > 0) delete m_text_renderer;
}