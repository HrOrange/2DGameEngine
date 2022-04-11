#pragma once
#include "common_includes.h"
#include "opengl_window.h"
#include "simple_shader.h"

class rectangle {
public:
	float m_x, m_y, m_width, m_height;

	rectangle(float x1, float y1, float w, float h, float color[4]);
	~rectangle();
	
	void draw();
	void set(float x, float y);
	void move(float x, float y);


private:

	const char* rectangle_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 aPos;

		uniform vec3 iCam;
		uniform vec2 iRect;

		void main()
		{
			gl_Position = vec4((aPos + iCam.xy + iRect) * iCam.z, 0.0, 1.0);
		})HERE";

	const char* rectangle_fragment_shader = R"HERE(
		#version 330 core
		
		uniform vec4 iColor;

		out vec4 FragColor;

		void main()
		{
			FragColor = iColor;
		}
		)HERE";

	simple_shader* m_shader;
	float m_color[4];
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint m_VAO, m_VBO, m_EBO;
	float m_ox, m_oy, m_mx, m_my; //t for translated, o for original and m for moved;
	GLint m_iCam_location, m_iRect_location;
};