#pragma once
#include "gif.h"

class button {
public:
	button(float x, float y, std::string gif_file_name);
	void draw();
	void move(float x, float y);
	void set_position(float x1, float y1, float x2, float y2);

private:
	int m_x, m_y, m_width, m_height;
	bool m_pressed;
	std::vector<image> images;
};