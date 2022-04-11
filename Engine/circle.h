#pragma once
#include "common_includes.h"
#include "opengl_window.h"
#include "simple_shader.h"

class circle {
public:
	float m_x, m_y, m_radius;
	float m_box[4];

	circle(float x, float y, float radius, float color[4]);
	~circle();
	void draw();

private:

	simple_shader* m_shader;
	float m_color[4];
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint m_VAO, m_VBO, m_EBO;
	GLint m_iCircle_location, m_iCam_location, m_iColor_location, m_iScreenSize_location;
	const char* circle_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 iPos;

		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((iPos + iCam.xy)* iCam.z, 0.0, 1.0);
		})HERE";

	const char* circle_fragment_shader = R"HERE(
		#version 330 core

		uniform vec3 iCam;
		uniform vec3 iCircle;
		uniform vec4 iColor;
		uniform vec3 iScreenSize;

		out vec4 FragColor;

		void main()
		{
			vec2 uv = gl_FragCoord.xy / iScreenSize.xy * 2.0 - 1.0 - (iCircle.xy + iCam.xy) * iCam.z;
			uv.x *= iScreenSize.z;
			float dist = length(uv / iCam.z);
			
			if(dist > iCircle.z) {
				discard;
			}
			FragColor = iColor;	
		}
		)HERE";

	/*
	vec2 uv = gl_FragCoord.xy / screenSize * 2.0 - 1.0 - iTransform.xy * iTransform.z;
	float dist = length(uv / iTransform.z);

	if(dist > iTransform.w) {
		FragColor = color;

	}
	*/

	float m_tx, m_ty, m_mx, m_my, m_tradius, m_tdiamater; //t for translated and m for moved
};