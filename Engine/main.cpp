//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Engine.h"
#include "global_variables.h"
#include "input.h"
#include <filesystem> 

//create a window object
opengl_window window(768, 768);
image* test_images;
void test_function(image* clicked_image, bool thing, int id) {
    println("hover: " + std::to_string(clicked_image->get_x()));
}
void test_drag_end_function(image* clicked_image, int id, std::vector<int> original_position) {
    println("dragged here: " + std::to_string(clicked_image->get_x(id)) + " : " + std::to_string(clicked_image->get_y(id)));

}


int main() {
    //initialize the window, global variables, GLFW, GLAD and other things
    std::string title = "test";
    float background_color[4] = { 0, 0, 255.0f, 255.0f };

    if (init(&window, title, background_color, 0) == -1) {
        println("ENGINE initialization failed.");
        return -1;
    }

    
    set_input_callback(&window, &key_callback);
    set_cursor_move_callback(&window, &cursor_position_callback);
    set_scroll_callback(&window, &scroll_callback);
    set_mouse_button_press_callback(&window, &mouse_button_press_callback);

    
    //initialize the audio module
    audio_manager->init(2, 0.5f, 0.5f);
    audio_manager->add_music("song", "test.wav", 1.0f);
    //AM.play_music("song");




    float solid_blue[4] =   { 0.0f,   0.0f,   255.0f, 255.0f };
    float solid_red[4] =    { 255.0f, 0.0f,   0.0f,   255.0f };
    float tran_red[4] =     { 255.0f, 0.0f,   0.0f,   55.0f  };
    float yellow[4] =       { 255.0f, 255.0f, 0.0f,   255.0f };
    float trans_yellow[4] = { 255.0f, 255.0f, 0.0f,   120.0f };
    float yellow_extra[4] = { 255.0f, 255.0f, 0.0f,   255.0f * 255.0f };
    
    
    FPS_counter FPS_m(0);

    //gif test_gif(100.0f, 100.0f, "lars");
    //text_database m_text_renderer("arial", 35);
    

    //batch
    std::vector<int> xs;
    std::vector<int> ys;
    std::vector<float> texture_s;    
    for (int j = 0; j < SCREEN_HEIGHT * 6; j += 128) {
        for (int i = 0; i < SCREEN_WIDTH * 6; i += 128) {

            xs.push_back(i);
            ys.push_back(j);
            //texture_s.push_back(0);
            float new_value = (float)i / 128.0f + (float)j / 128.0f;
            int new_value2 = (int)new_value % 2;
            //texture_s.push_back((float)new_value2);
            texture_s.push_back(get_random(0, 2));
        }
    }

    image* field_test = add_image(xs, ys, { "rock.png", "fog.png" }, texture_s, 0);


    image* TNT = add_image(512, 384, "TNT_trans.png", 1, mobility::camera_independent);
    //image* dwad = add_image(400, 256, "effect 3.png", 1, mobility::camera_independent);
    //image* dd = add_image(576, 448, "move 3.png", 1, mobility::camera_independent);
    


    std::vector<float> texture_t;
    for (int j = 0; j < SCREEN_HEIGHT; j += 128) {
        for (int i = 0; i < SCREEN_WIDTH; i += 128) {

            float new_value = (float)i / 128.0f + (float)j / 128.0f;
            int new_value2 = (int)new_value % 2;
            texture_t.push_back((float)new_value2);
        }
    }
    //field_test->switch_texture(texture_t);

    /* 
    //not batch
    for (float i = 0; i < SCREEN_WIDTH; i += 128) {
        for (float j = 0; j < SCREEN_HEIGHT; j += 128) {
            add_image(i, j, "field 128x128.png");
        }
    }
    */


    image* field_test21 = new image(50, 200, "rock.png");
    field_test21->m_shader.use();
    glUniform3f(glGetUniformLocation(field_test21->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);

    cam.set_position(128 * 6 * 3, 128 * 6 * 2, false);
    println(cam.m_t_camera_offset[0]);
    println(cam.m_t_camera_offset[1]);

    int thousand_count = 0;
    float color[3] = { 1.0f, 1.0f, 1.0f };
    // Loop until the user closes the window
    while (window.stay_open())
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        if (right) cam.move(delta_time, 1.0f, 0.0f);
        else if (left) cam.move(delta_time, -1.0f, 0.0f);
        if (up) { 
            cam.move(delta_time, 0.0f, 1.0f); 
        }
        else if (down) {
            cam.move(delta_time, 0.0f, -1.0f);
        }


        if (fullscreen_mode) {
            window.toggle_fullscreen();
            fullscreen_mode = false;
        }
        thousand_count++;
        if (thousand_count == 70) {
            //TNT->move(1.0f, 0);
            thousand_count = 0;
        }
        //TNT->set_position(MOUSE_X, MOUSE_Y, true);
        //TNT->hover(&test_function);
        //TNT->drag(&test_drag_end_function, true);

        //test_images->draw();
        for (auto& pair : GLOBAL_IMAGES) {
            for (auto& inner_pair : pair.second) {
                inner_pair.second->draw();
            }
        }
        field_test21->draw();
        FPS_m.update();

        //Render here
        //for(int i = 50.0f; i < 400.0f; i += 50.0f) m_text_renderer.render_text("[YEAH] thing", 150.0f, i, 1, yellow);

        //GLenum error = glGetError();
        //if (error) println(error);

        window.update();
        audio_manager->update();
    }
    //delete doad;
    delete test_images;

    if (quit() == -1) println("Encountered error when deleting variables saved on the heap");
    return 0;
}