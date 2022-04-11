#include "rectangle.h"
#include "global_variables.h"

rectangle::rectangle(float x, float y, float w, float h, float color[4])
	: m_mx(0),
	  m_my(0),
	  m_ox(x),
	  m_oy(y),
	  m_x(x),
	  m_y(y),
	  m_width(w),
	  m_height(h)
{
	for (int i = 0; i < 4; i++) m_color[i] = color[i] / 255.0f;


	//shader
	m_shader = new simple_shader();
	m_shader->init(rectangle_vertex_shader, rectangle_fragment_shader);
	m_shader->use();

	//get uniform locations
	m_iCam_location = glGetUniformLocation(m_shader->m_ID, "iCam");
	m_iRect_location = glGetUniformLocation(m_shader->m_ID, "iRect");

	//set uniforms here
	glUniform2f(m_iRect_location, m_mx, m_my);
	glUniform4f(glGetUniformLocation(m_shader->m_ID, "iColor"), m_color[0], m_color[1], m_color[2], m_color[3]);




	float tx1 = x / SCREEN_WIDTH * 2.0f - 1.0f;
	float ty1 = y / SCREEN_HEIGHT * -2.0f + 1.0f;
	float tx2 = (x + w) / SCREEN_WIDTH * 2.0f - 1.0f;
	float ty2 = (y + h) / SCREEN_HEIGHT * -2.0f + 1.0f;

	//set vertices
	float m_vertices[] = {
		// positions    // texture coords
		 tx2,  ty1, // top right
		 tx2,  ty2, // bottom right
		 tx1,  ty2, // bottom left
		 tx1,  ty1  // top left 
	};
	unsigned int m_indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	// set up vertex data (and buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_DYNAMIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


}

void rectangle::draw() {
	m_shader->use();
	glBindVertexArray(m_VAO);

	//set som uniforms
	//glUniform3f(m_iCam_location, cam.m_camera_offset[0], cam.m_camera_offset[1], cam.m_camera_zoom); //should probably do this in the camera

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

rectangle::~rectangle() {
	delete m_shader;
}

void rectangle::set(float x, float y) {
	m_mx = 0;
	m_my = 0;
	glUniform2f(m_iRect_location, m_mx, m_my);

	m_ox = x;
	m_oy = y;
	//should probably reset the whole m_vertices and buffer situation here to be more effective
	/*
	float tx1 = x / window.m_width * 2 - 1;
	float ty1 = y / window.m_height * -2 - 1;
	float tx2 = (x + m_width) / SCREEN_WIDTH * 2 - 1;
	float ty2 = (y + m_height) / SCREEN_HEIGHT * -2 - 1;

	//set vertices
	float m_vertices[] = {
		// positions    // texture coords
		 tx2,  ty1,    m_color[0], m_color[1], m_color[2], m_color[3], // top right
		 tx2,  ty2,    m_color[0], m_color[1], m_color[2], m_color[3], // bottom right
		 tx1,  ty2,    m_color[0], m_color[1], m_color[2], m_color[3], // bottom left
		 tx1,  ty1,    m_color[0], m_color[1], m_color[2], m_color[3] // top left 
	};


	// set up vertex data (and buffer(s)) and configure vertex attributes
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);
	*/


	m_x = x;
	m_y = y;
}
void rectangle::move(float x, float y) {
	m_mx += x;
	m_my += y;
	glUniform2f(m_iRect_location, m_mx, m_my);

	m_x += x;
	m_y += y;
}