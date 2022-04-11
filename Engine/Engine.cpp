#include "Engine.h"
#include "global_variables.h"


#include <Windows.h>
std::string getCurrentDir() {
    wchar_t  buff[MAX_PATH];
    GetModuleFileName(NULL, buff, MAX_PATH);
    std::wstring ws(buff);
    std::string total_path(ws.begin(), ws.end());
    std::vector<std::string> split_path;

    int last_i = 0;
    std::string checker("\\");
    for (int i = 1; i < total_path.length(); i++) {
        if (total_path.substr(i, 1) == checker) {
            split_path.push_back(total_path.substr(last_i, i - last_i));
            last_i = i + 1;
        }
    }
    split_path.push_back(total_path.substr(last_i, total_path.length() - last_i));

    std::string my_path(split_path[0]);
    int current_string = 1;
    while (split_path[current_string] != "x64") {
        my_path += "\\" + split_path[current_string];
        current_string++;
    }

    //std::string::size_type position = std::string(buff).find_last_of("\\/");
    //return std::string(buff).substr(0, position);
    return my_path;
}
int init(opengl_window* window, std::string title, float background_color[4], uint8_t fullscreen) {
    PATH = getCurrentDir();
    
    std::srand(static_cast <unsigned> (std::time(0))); //this is for the random number generator

    //initialize GLFW
    int res = GLFW_init();
    if (res == -1) {
        println("GLFW FAILED TO INITIALIZE");
        return -1;
    }

    //this initialization is annoying, but it needs to happen before GLAD initialization but after GLFW initialization
    window->init(window->m_width, window->m_height, title, fullscreen, background_color);
    SCREEN_WIDTH = window->m_width;
    SCREEN_HEIGHT = window->m_height;

    HALF_SCREEN_WIDTH = (floor(SCREEN_WIDTH / 2.0f));
    HALF_SCREEN_HEIGHT = (floor(SCREEN_HEIGHT / 2.0f));
    ASPECT_RATIO = SCREEN_HEIGHT / SCREEN_WIDTH;
    RECIPROCAL_ASPECT_RATIO = 1.0f / ASPECT_RATIO;

    cam.m_movement_speed /= HALF_SCREEN_WIDTH;

    //initialize GLAD
    res = GLAD_init();
    if (res == -1) {
        println("GLAD FAILED TO INITIALIZE");
        return -1;
    }

    //this enables transparency in OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    //Initialize fonts
    std::string fonts[] = { "arial", "BebasNeue-Regular" };
    for (int i = 0; i < sizeof(fonts) / sizeof(std::string); i++) FONTS.insert(std::pair<std::string, text_database*>(fonts[i], new text_database(fonts[i], 36)));

    return 0;
}
//this function is for clearing all the variables that have been saved on the heap and other things
int quit() {
    delete audio_manager;
    std::string fonts[] = { "arial", "BebasNeue-Regular" };
    for (int i = 0; i < sizeof(fonts) / sizeof(std::string); i++) {
        println(fonts[i]);
        delete FONTS[fonts[i]];
    }

    return 0;
}



image* add_image(int x, int y, std::string file_name, int index, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[index].size());
    /*
    std::string array_name;
    bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[index].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[index]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(int x, int y, std::string file_name, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[0].size());
    /*
    std::string array_name;
    bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[0].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[0]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);

    return new_image;
}

image* add_image(std::vector<int> x, std::vector<int> y, std::string file_name, int index, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[index].size());
    /*
    std::string array_name;
    bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[index].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[index]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(std::vector<int> x, std::vector<int> y, std::string file_name, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[0].size());
    /*
    std::string array_name;
    bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[0].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[0]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);

    return new_image;
}

image* add_image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, int index, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[index].size());
    /*
    std::string array_name;
    bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[index].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[index]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_names, texture_indexes, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map 



    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility_for_image) {
    std::string array_name = std::to_string(GLOBAL_IMAGES[0].size());
    /*bool not_done = true;
    for (int i = 0; i < GLOBAL_IMAGES[0].size(); i++) {
        array_name = std::to_string(i);
        for (auto& inner_pair : GLOBAL_IMAGES[0]) {
            if (inner_pair.first == array_name) {
                not_done = false;
                break;
            }
        }
        if (not_done) continue;

        break;
    }
    */
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_names, texture_indexes, chosen_mobility_for_image )});
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);
    }

    return new_image;
}



image* add_image(std::string array_name, int x, int y, std::string file_name, int index, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(std::string array_name, int x, int y, std::string file_name, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);

    return new_image;
}

image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::string file_name, int index, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::string file_name, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_name, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);

    return new_image;
}

image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, int index, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[index].insert({ array_name, new image(x, y, file_names, texture_indexes, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[index].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map


    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);


    return new_image;
}
image* add_image(std::string array_name, std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility_for_image) {
    GLOBAL_IMAGES[0].insert({ array_name, new image(x, y, file_names, texture_indexes, chosen_mobility_for_image) });
    image* new_image = (--GLOBAL_IMAGES[0].end())->second; //that whole (--map.end())->second/first gets the last value/key in a map

    new_image->m_shader.use();
    if (chosen_mobility_for_image == mobility::camera_dependent ||
        chosen_mobility_for_image == mobility::moveable) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], cam.m_camera_zoom);
    }
    else if (chosen_mobility_for_image == mobility::camera_move_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), cam.m_t_camera_offset[0], cam.m_t_camera_offset[1], 1.0f);
    }
    else if (chosen_mobility_for_image == mobility::camera_zoom_dependant) {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, cam.m_camera_zoom);
    }
    else {
        glUniform3f(glGetUniformLocation(new_image->m_shader.m_ID, "iCam"), 0.0f, 0.0f, 1.0f);
    }

    return new_image;
}