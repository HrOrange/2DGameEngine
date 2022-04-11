#include "line.h"
#include "global_variables.h"

line::line(const float& x1, const float& y1, const float& x2, const float& y2, const float& width, float color[4])
	: m_mx(0),
	  m_my(0),
	  m_x1(x1),
	  m_tx1(x1 / (float)SCREEN_WIDTH * 2.0f - 1.0f),
	  m_y1(y1),
	  m_ty1(y1 / (float)SCREEN_HEIGHT * 2.0f - 1.0f),
	  m_x2(x2),
	  m_tx2(x2 / (float)SCREEN_WIDTH * 2.0f - 1.0f),
	  m_y2(y2),
	  m_ty2(y2 / (float)SCREEN_HEIGHT * 2.0f - 1.0f),
	  m_width(width),
	  m_twidth(width / (float)std::min(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT)),
	  m_a(((SCREEN_HEIGHT - y2) - (SCREEN_HEIGHT - y1)) / (x2 - x1)),
	  m_b((SCREEN_HEIGHT - y1) - m_a * x1)
{
	if (m_x1 == m_x2 || m_y1 == m_y2) {
		m_rect = true;

		float max_x = std::max(x2, x1);
		float min_x = std::min(x2, x1);
		float max_y = std::max(y2, y1);
		float min_y = std::min(y2, y1);

		if (m_x1 == m_x2) my_rectangle = new rectangle(min_x - floor(m_width / 2), min_y, m_width, max_y - min_y, color);
		else my_rectangle = new rectangle(min_x, min_y - floor(m_width / 2), max_x - min_x, m_width, color);
	}
	else {
		for (int i = 0; i < 4; i++) m_color[i] = color[i] / 255.0f;

		//shader
		m_shader = new simple_shader();
		m_shader->init(line_rest_vertex_shader, line_rest_fragment_shader);


		//get uniform locations
		m_shader->use(); //the program needs to be used before uniforms can be set
		m_iLine_location = glGetUniformLocation(m_shader->m_ID, "iLine");
		m_iScreenSize_location = glGetUniformLocation(m_shader->m_ID, "iScreenSize");
		m_iColor_location = glGetUniformLocation(m_shader->m_ID, "iColor");


		//set uniform data
		//glUniform4f(glGetUniformLocation(m_shader->m_ID, "iEnd"), m_tx1, m_ty1, m_tx2, m_ty2);
		glUniform4f(m_iLine_location, m_a, m_b, m_width, sqrt(m_a * m_a + 1));
		glUniform4f(m_iColor_location, m_color[0], m_color[1], m_color[2], m_color[3]);
		glUniform3f(m_iScreenSize_location, SCREEN_WIDTH, SCREEN_HEIGHT, RECIPROCAL_ASPECT_RATIO);


		// set up vertex data (and buffer(s)) and configure vertex attributes
		/*float m_vertices[] = {
			// positions
			m_tx + m_tradius * ASPECT_RATIO,  m_ty + m_tradius, // top right
			m_tx + m_tradius * ASPECT_RATIO,  m_ty - m_tradius, // bottom right
			m_tx - m_tradius * ASPECT_RATIO,  m_ty - m_tradius, // bottom left
			m_tx - m_tradius * ASPECT_RATIO,  m_ty + m_tradius  // top left
		};*/

		float m_vertices[8];
		if (m_x2 > m_x1) {
			m_box[0] = m_x1;
			m_box[2] = m_x2;

			m_vertices[0] = (m_tx2 + m_twidth) * ASPECT_RATIO;
			m_vertices[2] = (m_tx2 + m_twidth) * ASPECT_RATIO;
			m_vertices[4] = (m_tx1 - m_twidth) * ASPECT_RATIO;
			m_vertices[6] = (m_tx1 - m_twidth) * ASPECT_RATIO;
		}
		else {
			m_box[0] = m_x2;
			m_box[2] = m_x1;

			m_vertices[0] = (m_tx1 + m_twidth) * ASPECT_RATIO;
			m_vertices[2] = (m_tx1 + m_twidth) * ASPECT_RATIO;
			m_vertices[4] = (m_tx2 - m_twidth) * ASPECT_RATIO;
			m_vertices[6] = (m_tx2 - m_twidth) * ASPECT_RATIO;
		}
		if (m_y2 > m_y1) {
			m_box[1] = m_y1;
			m_box[3] = m_y2;

			m_vertices[1] = m_ty1 - m_twidth; // top
			m_vertices[3] = m_ty2 + m_twidth; // bottom
			m_vertices[5] = m_ty2 + m_twidth; // bottom
			m_vertices[7] = m_ty1 - m_twidth; // top
		}
		else {
			m_box[1] = m_y2;
			m_box[3] = m_y1;

			m_vertices[1] = m_ty2 - m_twidth;
			m_vertices[3] = m_ty1 + m_twidth;
			m_vertices[5] = m_ty1 + m_twidth;
			m_vertices[7] = m_ty2 - m_twidth;
		}
		unsigned int m_indices[6] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};



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
	


}

void line::draw() {

	//println(std::to_string(m_box[0]) + " < " + std::to_string(cam.m_camera_box[2]) + " = " + std::to_string(m_box[0] < cam.m_camera_box[2]));
	//println(draw_now);


	if (m_rect) {
		my_rectangle->draw();
	}
	else {
		m_shader->use();
		glBindVertexArray(m_VAO);

		//set uniforms here
		//glUniform3f(m_iCam_location, cam.m_camera_offset[0], cam.m_camera_offset[1], cam.m_camera_zoom); //this is probably better to do in the camera function, but for now will it suffice

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

line::~line() {
	delete m_shader; //if this is called too early, then your created circles are probably being destroyed too early in the main code

	if (m_rect) delete my_rectangle;
}