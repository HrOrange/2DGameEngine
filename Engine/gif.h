#pragma once
#include "common_includes.h"
#include "image.h"

class gif {
public:
	float m_x1, m_y1, m_x2, m_y2;

	gif(float x, float y, std::string folder_name, float delay_time = 1.0f);
	~gif();
	void draw();
	void move(float x, float y);
	void set_position(float x1, float y1, float x2, float y2);

private:
	float m_tx1, m_ty1, m_tx2, m_ty2;
	int m_index, m_length;
	
	float m_delay_time, m_total_time;

	std::string m_folder_name;
	image* m_images;
};