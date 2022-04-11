#pragma once

/*
#ifdef ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
*/

extern bool fullscreen_mode;


//some global variables
#include "camera.h"
extern camera cam;
extern bool right;
extern bool left;
extern bool up;
extern bool down;
extern std::string PATH;
// these 4 variables could easily be int or unsigned int, but since they mostly get used to calculate floats, 
// have i chosen to make them floats
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;
extern float HALF_SCREEN_WIDTH;
extern float HALF_SCREEN_HEIGHT;
extern float ASPECT_RATIO;
extern float RECIPROCAL_ASPECT_RATIO;
extern float delta_time;
extern float total_time;
extern unsigned int MOUSE_X;
extern unsigned int MOUSE_Y;
extern float MOUSE_T_X;
extern float MOUSE_T_Y;
extern bool MOUSE_LEFT_PRESS;
extern bool MOUSE_RIGHT_PRESS;
extern bool MOUSE_MIDDLE_PRESS;
extern unsigned int MOUSE_LEFT_LAST_PRESS_X;
extern unsigned int MOUSE_LEFT_LAST_PRESS_Y;
extern unsigned int MOUSE_RIGHT_LAST_PRESS_X;
extern unsigned int MOUSE_RIGHT_LAST_PRESS_Y;
extern unsigned int MOUSE_MIDDLE_LAST_PRESS_X;
extern unsigned int MOUSE_MIDDLE_LAST_PRESS_Y;

extern const char* parenthesis[6];
#include "AudioManager.h"
extern AudioManager* audio_manager;
#include "text_database.h"
extern std::map<std::string, text_database*> FONTS;

//global objects
#include "image.h"
extern std::map<int, std::map<std::string, image*>> GLOBAL_IMAGES; 
//read up on std::atomic<variable type> since it seems to be a solution to a variable that can be changed from multiple threads
