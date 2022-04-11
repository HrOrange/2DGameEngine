#pragma once

//P = (x_1, y_1)
//L = ax + b
//d = |a * x_1 + b - y_1| / sqrt(a^2 + 1)

//der bliver behov for 3 shaders
//1 for vertikale linjer
//1 for horizontale linjer
//1 for alle andre linjer

#include "common_includes.h"
#include "opengl_window.h"
#include "simple_shader.h"
#include "rectangle.h"

class line {
public:
	float m_x1, m_y1, m_x2, m_y2, m_width;
	float m_box[4];

	line(const float& x1, const float& y1, const float& x2, const float& y2, const float& width, float color[4]);
	~line();
	void draw();


private:

	simple_shader* m_shader;
	float m_color[4];
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint m_VAO, m_VBO, m_EBO;
	GLint m_iLine_location, m_iCam_location, m_iColor_location, m_iScreenSize_location;
	const char* line_rest_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 iPos;

		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((iPos + iCam.xy)* iCam.z, 0.0, 1.0);
		})HERE";

	const char* line_rest_fragment_shader = R"HERE(
		#version 330 core

		uniform vec3 iCam;
		uniform vec4 iColor;
		uniform vec3 iScreenSize;
		uniform vec4 iLine; 

		out vec4 FragColor;

		void main()
		{
			

			float dist_1 = abs(iLine.x * gl_FragCoord.x + iLine.y - gl_FragCoord.y) / iLine.w;
			
			if(dist_1 > iLine.z) {
				discard;
			}

			FragColor = iColor;
		}
		)HERE";
	/*
	uniform vec4 iEnd;
	float dist_2 = min(length(iEnd.xy - gl_FragCoord.xy), length(vec2(iEnd.zw - gl_FragCoord.xy)));

	*/

	bool m_rect = false;
	rectangle* my_rectangle;
	float m_tx1, m_ty1, m_tx2, m_ty2, m_mx, m_my, m_twidth; //t for translated and m for moved
	float m_a, m_b; //a and b from ax + b
};