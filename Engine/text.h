#pragma once
#include "common_includes.h"
#include "text_database.h"
#include "opengl_window.h"


class text {
public:
	text(text_database* my_text_database, std::string my_text, int x, int y, float color[4], float scale = 1.0f, float space = 0.02f, unsigned int vertical_align = 2);
	~text();

	void draw();
	void set_text(std::string new_text);

private:
	text_database* m_database;
	std::string m_text, m_previous_text;
	float m_x, m_y;
	std::vector<float> m_tx;
	std::vector<float> m_ty;
	std::vector<float*> m_vertices;
	int m_vertices_size, m_vertical_align; //m_vertical_align = 0 (text in top),  m_vertical_align = 1 (text in middle),  m_vertical_align = 2 (text in bottom)
	float m_color[4], m_scale, m_space;
};