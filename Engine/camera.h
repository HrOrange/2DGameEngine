#pragma once
#include "common_includes.h"

class camera {
public:
	float m_reciprocal_zoom = 1.0f;
	float m_camera_zoom = 0.0f;
	float m_camera_moved[2]; //this variable controls how much the camera have been moved, so the offset and camera_box doesn't have to
	float m_t_camera_offset[2];
	//float m_movement_speed = 0.5f; //this is measured in how many pixels gets traversed by holding the button in for 1 second
	float m_movement_speed = 240.0f; //this is measured in how many pixels gets traversed by holding the button in for 1 second

	camera();
	camera(float zoom);
	~camera();
	void move(float delta, float x, float y);
	void set_position(int x, int y, bool center = true);
	void zoom(float amount);


private:
	float m_scroll_scale = 0.01f;
	float m_camera_clamp[2];
};