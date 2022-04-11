#pragma once
#include "common_includes.h"
#include "opengl_window.h"
#include "simple_shader.h"

class triangle {
public:
	float m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;

	triangle(float x1, float y1, float x2, float y2, float x3, float y3, float color[4], opengl_window* window);
	~triangle();
	void draw();


private:

	simple_shader* m_shader;
	float m_color[4];
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint m_VAO, m_VBO;
	const char* triangle_vertex_shader = R"HERE(
		#version 330 core

		layout(location = 0) in vec2 aPos;

		uniform vec4 aColor;
		out vec4 color;


		void main()
		{
			gl_Position = vec4 (aPos.x, aPos.y, 0, 1);
			color = aColor;
		})HERE";

	const char* triangle_fragment_shader = R"HERE(
		#version 330 core

		out vec4 FragColor;


		in vec4 color;


		void main()
		{
			FragColor = color;
		}
		)HERE";
};