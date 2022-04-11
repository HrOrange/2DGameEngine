#pragma once
#include "common_includes.h"
#include "opengl_window.h"
#include "simple_shader.h"
#include <stb/stb_image.h>

enum class mobility {
	//for UI elements
	stationary, //cannot move, is not affected by camera move or zoom
	
	//for field
	camera_dependent, //cannot move, but is affected by camera move and zoom
	
	//for cursor
	camera_zoom_dependant, //cannot move, but is affected by camera zoom

	//for something
	camera_move_dependant, //cannot move, but is affected by camera move
	
	moveable, //can move, is affected by camera move and zoom

	//for cards
	camera_independent //can move, is not affected by camera move or zoom
};

class image {
public:
	std::vector<uint8_t> m_numColChannels;
	uint16_t m_object_count, m_width, m_height; //max of 65536
	std::vector<float> m_texture_indexes;
	std::vector<bool> m_hover_toggle, m_click_toggle, m_drag; 
	simple_shader m_shader;
	mobility m_mobility;

	image();
	image(int x, int y, std::string file_name = "", mobility chosen_mobility = mobility::camera_dependent);
	image(std::vector<int> x, std::vector<int> y, std::string file_name = "", mobility chosen_mobility = mobility::camera_dependent);
	image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility = mobility::camera_dependent);
	~image();
	inline int get_x(int id = 0) { return m_x1[id]; }
	inline int get_y(int id = 0) { return m_y1[id]; }
	inline int get_x2(int id = 0) { return m_x2[id]; }
	inline int get_y2(int id = 0) { return m_y2[id]; }
	inline float get_t_x(int id = 0) { return m_tx1[id]; }
	inline float get_t_y(int id = 0) { return m_ty1[id]; }
	inline float get_t_x2(int id = 0) { return m_tx2[id]; }
	inline float get_t_y2(int id = 0) { return m_ty2[id]; }
	inline uint8_t get_color_channels(int id = 0) { return m_numColChannels[id]; }
	void init(int x, int y, std::string file_name = "", mobility chosen_mobility = mobility::camera_dependent);
	void draw();
	void move(int x, int y, int object = 0);
	void set_position(int x, int y, bool center = false, int object = 0);
	void hover(void (*on_hover_function)(image*, bool, int), bool toggle = true);
	void click(void (*on_click_function)(image*, int), bool toggle = true);
	bool drag(void (*on_drag_end_function)(image*, int, std::vector<int>), bool center = true);
	void switch_texture(std::vector<float> texture_indexes);

	std::vector<float> m_tx1, m_ty1, m_tx2, m_ty2;
private:
	uint8_t m_texture_count, m_vertices_count;
	std::vector<float> m_vertices;
	std::vector<std::vector<int>> m_original_positions;
	int m_saved_mouse_position[2];
	int m_shaderID;
	int m_indecesNum = 6;
	GLuint m_VAO, m_VBO, m_EBO;
	std::vector<GLuint> m_textureID;
	int* m_colorData;
	std::vector<int> m_x1, m_y1, m_x2, m_y2;
	
}; 