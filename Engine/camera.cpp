#include "camera.h"
#include "global_variables.h"

camera::camera() 
	: m_camera_clamp{ 0.1f, 2.0f },
	  m_camera_zoom(1.0f)
{
	//for (int i = 0; i < 4; i++) println(m_camera_box[i]);
}
camera::camera(float zoom)
	: m_camera_clamp{ 0.1f, 2.0f },
	  m_camera_zoom(zoom)
{
	//for (int i = 0; i < 4; i++) println(m_camera_box[i]);
}


camera::~camera() {

}

void camera::set_position(int x, int y, bool center) {


	if (center) {
		m_camera_moved[0] = -(x - HALF_SCREEN_WIDTH)  / HALF_SCREEN_WIDTH  * m_reciprocal_zoom / m_movement_speed;
		m_camera_moved[1] =  (y - HALF_SCREEN_HEIGHT) / HALF_SCREEN_HEIGHT * m_reciprocal_zoom / m_movement_speed;
	}
	else {
		m_camera_moved[0] = -x / HALF_SCREEN_WIDTH  * m_reciprocal_zoom / m_movement_speed;
		m_camera_moved[1] =  y / HALF_SCREEN_HEIGHT * m_reciprocal_zoom / m_movement_speed;
	}

	//first move things in screen space (x = -1 -> 1    y = -1 -> 1)
	m_t_camera_offset[0] = m_camera_moved[0] * m_movement_speed; //* ASPECT_RATIO;
	m_t_camera_offset[1] = m_camera_moved[1] * m_movement_speed;
	println(m_t_camera_offset[0]);
	println(m_t_camera_offset[1]);

	/*
	//for debugging
	println("-------");
	println(m_camera_offset[0]);
	println(m_camera_offset[1]);
	println(m_t_camera_offset[0]);
	println(m_t_camera_offset[1]);
	println("");
	println(m_camera_zoom);
	println(RECIPROCAL_ASPECT_RATIO);
	println(ASPECT_RATIO);
	*/

	//update objects in scene
	for (auto& pair : GLOBAL_IMAGES) {
		for (auto& inner_pair : pair.second) {
			if (inner_pair.second->m_mobility == mobility::camera_dependent ||
				inner_pair.second->m_mobility == mobility::camera_move_dependant ||
				inner_pair.second->m_mobility == mobility::moveable) {
				inner_pair.second->m_shader.use();
				glUniform3f(glGetUniformLocation(inner_pair.second->m_shader.m_ID, "iCam"), m_t_camera_offset[0], m_t_camera_offset[1], m_camera_zoom);
			}
		}
	}
}
void camera::move(float delta, float x, float y) {
	//m_camera_moved[0] -= x * delta; //* m_camera_zoom; //good effect, but really annoying when zoom all out
	//m_camera_moved[1] -= y * delta; //* m_camera_zoom; //good effect, but really annoying when zoom all out

	m_camera_moved[0] -= x * delta * m_reciprocal_zoom;
	m_camera_moved[1] -= y * delta * m_reciprocal_zoom * RECIPROCAL_ASPECT_RATIO;
	
	//first move things in screen space (x = -1 -> 1    y = -1 -> 1)
	float movement_x = m_camera_moved[0] * m_movement_speed; //* ASPECT_RATIO;
	float movement_y = m_camera_moved[1] * m_movement_speed;

	m_t_camera_offset[0] = movement_x;
	m_t_camera_offset[1] = movement_y;

	//then in pixel space (x = 0 -> 1920    y = 0 -> 1080) //not used at the moment, since pretty much all camera data that is used is in -1 -> 1 format (screen format or something)
	//movement_x *= HALF_SCREEN_WIDTH;
	//movement_y *= HALF_SCREEN_HEIGHT;
	
	
	/*
	float movement_x = -x * m_movement_speed * delta * m_reciprocal_zoom; //* ASPECT_RATIO;
	float movement_y = -y * m_movement_speed * delta * m_reciprocal_zoom * RECIPROCAL_ASPECT_RATIO;

	m_t_camera_offset[0] += movement_x;
	m_t_camera_offset[1] += movement_y;
	
	movement_x *= HALF_SCREEN_WIDTH;
	movement_y *= HALF_SCREEN_HEIGHT;
	m_camera_offset[0] -= movement_x;
	m_camera_offset[1] -= movement_y;
	m_camera_box[0] -= movement_x;
	m_camera_box[2] -= movement_x;
	m_camera_box[1] -= movement_y;
	m_camera_box[3] -= movement_y;
	*/

	for (auto& pair : GLOBAL_IMAGES) {
		for (auto& inner_pair : pair.second) {
			if (inner_pair.second->m_mobility == mobility::camera_dependent ||
				inner_pair.second->m_mobility == mobility::camera_move_dependant ||
				inner_pair.second->m_mobility == mobility::moveable) {
				inner_pair.second->m_shader.use();
				glUniform3f(glGetUniformLocation(inner_pair.second->m_shader.m_ID, "iCam"), m_t_camera_offset[0], m_t_camera_offset[1], m_camera_zoom);
			}
		}
	}

}

void camera::zoom(float amount) {
	if (amount > 0) {
		if (m_camera_zoom + amount * m_scroll_scale < m_camera_clamp[1]) m_camera_zoom += amount * m_scroll_scale;
		else m_camera_zoom = m_camera_clamp[1];
	}
	else {
		if (m_camera_zoom + amount * m_scroll_scale > m_camera_clamp[0]) m_camera_zoom += amount * m_scroll_scale;
		else m_camera_zoom = m_camera_clamp[0];
	}
	
	//now reset offset values and other variables
	m_reciprocal_zoom = 1.0f / m_camera_zoom;
	
	//m_t_camera_offset[0] = m_camera_moved[0] * m_movement_speed; //* ASPECT_RATIO;
	//m_t_camera_offset[1] = m_camera_moved[1] * m_movement_speed * RECIPROCAL_ASPECT_RATIO;
	//println(m_t_camera_offset[0]);
	//println(m_t_camera_offset[1]);





	//then in pixel space (x = 0 -> 1920    y = 0 -> 1080) //not used at the moment, since pretty much all camera data that is used is in -1 -> 1 format (screen format or something)
	//float movement_x = m_camera_moved[0] * m_movement_speed * HALF_SCREEN_WIDTH; //* ASPECT_RATIO;
	//float movement_y = m_camera_moved[1] * m_movement_speed * HALF_SCREEN_HEIGHT * RECIPROCAL_ASPECT_RATIO;



	/* If i ever wanna have the box around camera data
	m_camera_box[0] = -m_reciprocal_zoom * HALF_SCREEN_WIDTH + m_camera_offset[0];
	m_camera_box[1] = m_reciprocal_zoom * HALF_SCREEN_HEIGHT + m_camera_offset[1];
	m_camera_box[2] = m_reciprocal_zoom * HALF_SCREEN_WIDTH + m_camera_offset[0];
	m_camera_box[3] = -m_reciprocal_zoom * HALF_SCREEN_HEIGHT + m_camera_offset[1];
	*/


	for (auto& pair : GLOBAL_IMAGES) {
		for (auto& inner_pair : pair.second) {
			if (inner_pair.second->m_mobility == mobility::camera_dependent ||
				inner_pair.second->m_mobility == mobility::camera_move_dependant ||
				inner_pair.second->m_mobility == mobility::moveable) {
				inner_pair.second->m_shader.use();
				glUniform3f(glGetUniformLocation(inner_pair.second->m_shader.m_ID, "iCam"), m_t_camera_offset[0], m_t_camera_offset[1], m_camera_zoom);
			}
		}
	}
}