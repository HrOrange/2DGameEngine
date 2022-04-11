#include "common_includes.h"
#include "global_variables.h"

bool fullscreen_mode = false;

// settings
float SCREEN_WIDTH = 500;
float SCREEN_HEIGHT = 500;
float HALF_SCREEN_WIDTH = (floor(SCREEN_WIDTH / 2.0f));
float HALF_SCREEN_HEIGHT = (floor(SCREEN_HEIGHT / 2.0f));
float ASPECT_RATIO = SCREEN_HEIGHT / SCREEN_WIDTH;
float RECIPROCAL_ASPECT_RATIO = 1.0f / ASPECT_RATIO;
unsigned int MOUSE_X = 0;
unsigned int MOUSE_Y = 0;
float MOUSE_T_X = 0;
float MOUSE_T_Y = 0;
extern bool MOUSE_LEFT_PRESS = false;
extern bool MOUSE_RIGHT_PRESS = false;
extern bool MOUSE_MIDDLE_PRESS = false;
extern unsigned int MOUSE_LEFT_LAST_PRESS_X = 0;
extern unsigned int MOUSE_LEFT_LAST_PRESS_Y = 0;
extern unsigned int MOUSE_RIGHT_LAST_PRESS_X = 0;
extern unsigned int MOUSE_RIGHT_LAST_PRESS_Y = 0;
extern unsigned int MOUSE_MIDDLE_LAST_PRESS_X = 0;
extern unsigned int MOUSE_MIDDLE_LAST_PRESS_Y = 0;
camera cam; //camera cam(0.0f); // i can initialize the variabel here, but i dont really need to initialize it in any specific way
bool right, left, up, down;
float delta_time;
float total_time;
const char* parenthesis[6] = { "(", ")", "[", "]", "{", "}" };
AudioManager* audio_manager = new AudioManager();
std::map<std::string, text_database*> FONTS;
std::string PATH;

std::map<int, std::map<std::string, image*>> GLOBAL_IMAGES = { {-1, {}}, {0, {}}, {1, {}}, {2, {}} };