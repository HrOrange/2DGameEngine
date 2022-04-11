#include "triangle.h"

triangle::triangle(float x1, float y1, float x2, float y2, float x3, float y3, float color[4], opengl_window* window)
	: m_x1((x1 / window->m_width - 0.5) * -2),
	  m_y1((y1 / window->m_height - 0.5) * -2),
	  m_x2((x2 / window->m_width - 0.5) * -2),
	  m_y2((y2 / window->m_height - 0.5) * -2),
	  m_x3((x3 / window->m_width - 0.5) * -2),
	  m_y3((y3 / window->m_height - 0.5) * -2) {
	for (int i = 0; i < 4; i++) m_color[i] = color[i];


	m_shader->init(triangle_vertex_shader, triangle_fragment_shader);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	GLfloat vertices[6] =
	{
		m_x1, m_y1, 
		m_x2, m_y2, 
		m_x3, m_y3
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);
	
}

void triangle::draw() {
	m_shader->use();
	glBindVertexArray(m_VAO);

	int colorLocation = glGetUniformLocation(m_shader->m_ID, "aColor");
	glUniform4f(colorLocation, m_color[0], m_color[1], m_color[2], m_color[3]);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

triangle::~triangle() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteProgram(m_shader->m_ID);
}