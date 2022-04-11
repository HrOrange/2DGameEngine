#include "circle.h"
#include "global_variables.h"

circle::circle(float x, float y, float radius, float color[4])
	: m_mx(0),
	  m_my(0),
	  m_x(x),
	  m_tx((x / SCREEN_WIDTH  - 0.5f) *  2.0f),
	  m_y(y),
	  m_ty((y / SCREEN_HEIGHT - 0.5f) * -2.0f),
	  m_radius(radius),
	  m_box{ m_tx * SCREEN_WIDTH - radius, m_ty * SCREEN_HEIGHT + radius, m_tx * SCREEN_WIDTH + radius, m_ty * SCREEN_HEIGHT - radius },
	  m_tradius(radius / (float)std::min(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT)),
	  m_tdiamater(m_tradius * 2.0f)
{
	for (int i = 0; i < 4; i++) m_color[i] = color[i] / 255.0f;
	//for (int i = 0; i < 4; i++) println(m_box[i]);

	//shader
	m_shader = new simple_shader();
	m_shader->init(circle_vertex_shader, circle_fragment_shader);
	
	//get uniform locations
	m_shader->use(); //the program needs to be used before uniforms can be set
	m_iCircle_location = glGetUniformLocation(m_shader->m_ID, "iCircle");
	m_iScreenSize_location = glGetUniformLocation(m_shader->m_ID, "iScreenSize");
	m_iColor_location = glGetUniformLocation(m_shader->m_ID, "iColor");

	//set uniform data
	glUniform3f(m_iCircle_location, m_tx, m_ty, m_tradius);
	glUniform4f(m_iColor_location, m_color[0], m_color[1], m_color[2], m_color[3]);
	glUniform3f(m_iScreenSize_location, SCREEN_WIDTH, SCREEN_HEIGHT, RECIPROCAL_ASPECT_RATIO);

	float m_vertices[] = {
		// positions										
		m_tx + m_tradius * ASPECT_RATIO,  m_ty + m_tradius, // top right
		m_tx + m_tradius * ASPECT_RATIO,  m_ty - m_tradius, // bottom right
		m_tx - m_tradius * ASPECT_RATIO,  m_ty - m_tradius, // bottom left
		m_tx - m_tradius * ASPECT_RATIO,  m_ty + m_tradius  // top left 
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

	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	

}

void circle::draw() {
	
	m_shader->use();
	glBindVertexArray(m_VAO);

	//set uniforms here
	//glUniform3f(m_iCam_location, cam.m_camera_offset[0], cam.m_camera_offset[1], cam.m_camera_zoom); //this is probably better to do in the camera function, but for now will it suffice
		
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

circle::~circle() { 
	delete m_shader; //if this is called too early, then your created circles are probably being destroyed too early in the main code
}