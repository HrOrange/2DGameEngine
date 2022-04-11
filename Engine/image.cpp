#include "image.h"
#include "global_variables.h"

std::tuple<GLuint, int, int, int> LoadTexture(std::string file_name) {
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* bytes = stbi_load((PATH + "\\" + file_name).c_str(), &w, &h, &bits, 0);
	println(file_name + " : " + std::to_string(bits));

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	if (bits == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	else if (bits == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	stbi_image_free(bytes);

	return { textureID, w, h, bits };
}

image::image() 
  : m_object_count(1),
	m_texture_count(1),
	m_hover_toggle(1),
	m_click_toggle(1),
	m_drag(1),
	m_original_positions({ {0, 0} }),
	m_vertices_count(16) {

}
image::image(int x, int y, std::string file_name, mobility chosen_mobility)
	: m_mobility(chosen_mobility),
	  m_object_count(1),
	  m_texture_count(1),
	  m_hover_toggle(1),
	  m_drag(1),
	  m_click_toggle(1),
	  m_original_positions({ {x, y} }),
	  m_vertices_count(16) {
	  m_tx1.push_back((x / SCREEN_WIDTH - 0.5f) * 2.0f);
	  m_ty1.push_back((y / SCREEN_HEIGHT - 0.5f) * -2.0f);
	  m_x1.push_back(x);
	  m_y1.push_back(y);

	//shader
	const char* image_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;
		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((aPos + iCam.xy) * iCam.z, 0.0, 1.0);
			TexCoord = aTexCoord;
		})HERE";

	const char* image_fragment_shader = R"HERE(
		#version 330 core
		out vec4 FragColor;
	
		in vec2 TexCoord;

		uniform sampler2D ourTexture;

		void main()
		{
			FragColor = texture(ourTexture, TexCoord);
		})HERE";
	m_shader.init(image_vertex_shader, image_fragment_shader);
	

	//load texture
	auto [textureID, w, h, bits] = LoadTexture(file_name);
	m_textureID.push_back(textureID);
	m_width = w;
	m_height = h;
	m_numColChannels.push_back(bits);

	m_x2.push_back(m_x1[0] + m_width);
	m_y2.push_back(m_y1[0] + m_height);
	m_tx2.push_back((m_x2[0] / SCREEN_WIDTH - 0.5f) * 2.0f);
	m_ty2.push_back((m_y2[0] / SCREEN_HEIGHT - 0.5f) * -2.0f);



	// set up vertex data (and buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	/*
	float vertices[16] = {
		// positions    // texture coords
		 0.5f,  0.5f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	*/
	m_vertices = {
		// positions    // texture coords
		 m_tx2[0], m_ty1[0],   1.0f, 1.0f,   // top right
		 m_tx2[0], m_ty2[0],   1.0f, 0.0f,   // bottom right
		 m_tx1[0], m_ty2[0],   0.0f, 0.0f,   // bottom left
		 m_tx1[0], m_ty1[0],   0.0f, 1.0f    // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (m_mobility == mobility::moveable || m_mobility == mobility::camera_independent) glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

}
image::image(std::vector<int> x, std::vector<int> y, std::string file_name, mobility chosen_mobility)
	: m_x1(x),
	  m_y1(y),
	  m_mobility(chosen_mobility),
	  m_texture_count(1),
	  m_object_count(x.size()),
	  m_hover_toggle(m_object_count),
	  m_click_toggle(m_object_count),
	  m_drag(m_object_count),
	  m_vertices(m_object_count * 16),
	  m_vertices_count(16) {
	m_x2.resize(m_object_count);
	m_y2.resize(m_object_count);

	m_tx1.resize(m_object_count);
	m_tx2.resize(m_object_count);
	m_ty1.resize(m_object_count);
	m_ty2.resize(m_object_count);
	for (int i = 0; i < m_object_count; i++) m_original_positions.push_back({ m_x1[i], m_y1[i] });


	//shader
	const char* image_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;
		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((aPos + iCam.xy) * iCam.z, 0.0, 1.0);
			TexCoord = aTexCoord;
		})HERE";

	const char* image_fragment_shader = R"HERE(
		#version 330 core
		out vec4 FragColor;
	
		in vec2 TexCoord;

		uniform sampler2D ourTexture;

		void main()
		{
			FragColor = texture(ourTexture, TexCoord);
		})HERE";

	m_shader.init(image_vertex_shader, image_fragment_shader);



	//load texture
	auto [textureID, w, h, bits] = LoadTexture(file_name);
	m_textureID.push_back(textureID);
	m_width = w;
	m_height = h;
	m_numColChannels.push_back(bits);
	

	// set up vertex data (and buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	for (int i = 0; i < m_object_count; i++) {
		m_tx1[i] = (m_x1[i] / SCREEN_WIDTH - 0.5f) * 2.0f;
		m_ty1[i] = (m_y1[i] / SCREEN_HEIGHT - 0.5f) * -2.0f;

		m_x2[i] = x[i] + m_width;
		m_y2[i] = y[i] + m_height;
		m_tx2[i] = (m_x2[i] / SCREEN_WIDTH - 0.5f) * 2.0f;
		m_ty2[i] = (m_y2[i] / SCREEN_HEIGHT - 0.5f) * -2.0f;
	}
	
	m_indecesNum = 6 * m_object_count;
	std::vector<unsigned int> indices(m_object_count * 6, 0);

	for (int i = 0; i < m_object_count; i++) {
		int index = i * 16;
		m_vertices[index] = m_tx2[i];
		m_vertices[index + 1] = m_ty1[i];
		m_vertices[index + 2] = 1.0f;
		m_vertices[index + 3] = 1.0f;
		
		m_vertices[index + 4] = m_tx2[i];
		m_vertices[index + 5] = m_ty2[i];
		m_vertices[index + 6] = 1.0f;
		//vertices[index + 7] = 0.0f;
		
		m_vertices[index + 8] = m_tx1[i];
		m_vertices[index + 9] = m_ty2[i];
		//vertices[index + 10] = 0.0f;
		//vertices[index + 11] = 0.0f;
		
		m_vertices[index + 12] = m_tx1[i];
		m_vertices[index + 13] = m_ty1[i];
		//vertices[index + 14] = 0.0f;
		m_vertices[index + 15] = 1.0f;


		int index_2 = i * 6;
		int current_count = i * 4;
		indices[index_2] = current_count;
		indices[index_2 + 1] = current_count + 1;
		indices[index_2 + 2] = current_count + 3;
		indices[index_2 + 3] = current_count + 1;
		indices[index_2 + 4] = current_count + 2;
		indices[index_2 + 5] = current_count + 3;
	}

	/*
	float vertices[16] = {
		// positions    // texture coords
		 m_tx2, m_ty1,   1.0f, 1.0f,   // top right
		 m_tx2, m_ty2,   1.0f, 0.0f,   // bottom right
		 m_tx1, m_ty2,   0.0f, 0.0f,   // bottom left
		 m_tx1, m_ty1,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	*/

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (m_mobility == mobility::moveable || m_mobility == mobility::camera_independent) glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);



	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


}
image::image(std::vector<int> x, std::vector<int> y, std::vector<std::string> file_names, std::vector<float> texture_indexes, mobility chosen_mobility)
	: m_x1(x),
	  m_y1(y),
	  m_mobility(chosen_mobility),
	  m_texture_count(file_names.size()),
	  m_object_count(x.size()),
	  m_hover_toggle(m_object_count),
	  m_click_toggle(m_object_count),
	  m_drag(m_object_count),
	  m_vertices(m_object_count * 20),
	  m_vertices_count(20),
	  m_texture_indexes(texture_indexes) {

	m_x2.resize(m_object_count);
	m_y2.resize(m_object_count);

	m_tx1.resize(m_object_count);
	m_tx2.resize(m_object_count);
	m_ty1.resize(m_object_count);
	m_ty2.resize(m_object_count);
	for (int i = 0; i < m_object_count; i++) m_original_positions.push_back({ m_x1[i], m_y1[i] });


	//shader
	std::string image_vertex_shader = R"""(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTexCoord;
		layout (location = 2) in float aTexIndex;

		out vec2 TexCoord;
		out float oTexIndex;
		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((aPos + iCam.xy) * iCam.z, 0.0, 1.0);
			TexCoord = aTexCoord;
			oTexIndex = aTexIndex;
		})""";

	std::string image_fragment_shader = R"""(
		#version 330 core 
		out vec4 FragColor; 
	
		in vec2 TexCoord;
		in float oTexIndex;
	
		uniform sampler2D ourTextures[)""" + std::to_string(m_texture_count) + R"""(]; 
	
		void main()
		{
			int index = int(oTexIndex);
			FragColor = texture(ourTextures[index], TexCoord);
		})""";

	m_shader.init(image_vertex_shader.c_str(), image_fragment_shader.c_str());

	/*
	* int index = int(oTexIndex);
	* FragColor = texture(ourTextures[index], TexCoord);
	*/


	//load textures
	for (int i = 0; i < file_names.size(); i++) {
		auto [textureID, w, h, bits] = LoadTexture(file_names[i]);
		m_textureID.push_back(textureID);
		m_width = w;
		m_height = h;
		m_numColChannels.push_back(bits);

	}
	
	m_shader.use(); //cannot set uniforms without having used the shader
	
	GLint loc = glGetUniformLocation(m_shader.m_ID, "ourTextures");
	std::vector<int> sampler(m_texture_count);
	for (int i = 0; i < m_texture_count; i++) sampler[i] = i;
	glUniform1iv(loc, m_texture_count, sampler.data());

	// set up vertex data (and buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	for (int i = 0; i < m_object_count; i++) {
		m_tx1[i] = (m_x1[i] / SCREEN_WIDTH - 0.5f) * 2.0f;
		m_ty1[i] = (m_y1[i] / SCREEN_HEIGHT - 0.5f) * -2.0f;

		m_x2[i] = x[i] + m_width;
		m_y2[i] = y[i] + m_height;
		m_tx2[i] = (m_x2[i] / SCREEN_WIDTH - 0.5f) * 2.0f;
		m_ty2[i] = (m_y2[i] / SCREEN_HEIGHT - 0.5f) * -2.0f;
	}

	m_indecesNum = 6 * m_object_count;
	std::vector<unsigned int> indices(m_object_count * 6, 0);

	for (int i = 0; i < m_object_count; i++) {

		int index = i * 20;
		m_vertices[index] = m_tx2[i];
		m_vertices[index + 1] = m_ty1[i];
		m_vertices[index + 2] = 1.0f;
		m_vertices[index + 3] = 1.0f;
		m_vertices[index + 4] = texture_indexes[i];

		m_vertices[index + 5] = m_tx2[i];
		m_vertices[index + 6] = m_ty2[i];
		m_vertices[index + 7] = 1.0f;
		//vertices[index + 8] = 0.0f;
		m_vertices[index + 9] = texture_indexes[i];

		m_vertices[index + 10] = m_tx1[i];
		m_vertices[index + 11] = m_ty2[i];
		//vertices[index + 12] = 0.0f;
		//vertices[index + 13] = 0.0f;
		m_vertices[index + 14] = texture_indexes[i];

		m_vertices[index + 15] = m_tx1[i];
		m_vertices[index + 16] = m_ty1[i];
		//vertices[index + 17] = 0.0f;
		m_vertices[index + 18] = 1.0f;
		m_vertices[index + 19] = texture_indexes[i];


		int index_2 = i * 6;
		int current_count = i * 4;
		indices[index_2] = current_count;
		indices[index_2 + 1] = current_count + 1;
		indices[index_2 + 2] = current_count + 3;
		indices[index_2 + 3] = current_count + 1;
		indices[index_2 + 4] = current_count + 2;
		indices[index_2 + 5] = current_count + 3;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (m_mobility == mobility::moveable || m_mobility == mobility::camera_independent) glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	
}


void image::init(int x, int y, std::string file_name, mobility chosen_mobility) {
	m_tx1.push_back((x / SCREEN_WIDTH - 0.5) * 2);
	m_ty1.push_back((y / SCREEN_HEIGHT - 0.5) * -2);
	m_x1.push_back(x);
	m_y1.push_back(y);
	m_mobility = chosen_mobility;


	//shader
	const char* image_vertex_shader = R"HERE(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;
		uniform vec3 iCam;

		void main()
		{
			gl_Position = vec4((aPos + iCam.xy) * iCam.z, 0.0, 1.0);
			TexCoord = aTexCoord;
		})HERE";

	const char* image_fragment_shader = R"HERE(
		#version 330 core
		out vec4 FragColor;
	
		in vec2 TexCoord;

		uniform sampler2D ourTexture;

		void main()
		{
			FragColor = texture(ourTexture, TexCoord);
		})HERE";
	m_shader.init(image_vertex_shader, image_fragment_shader);



	//load texture
	auto [textureID, w, h, bits] = LoadTexture(file_name);
	m_textureID.push_back(textureID);
	m_width = w;
	m_height = h;
	m_numColChannels.push_back(bits);

	m_x2.push_back(m_x1[0] + m_width);
	m_y2.push_back(m_y1[0] + m_height);
	m_tx2.push_back((m_x2[0] / SCREEN_WIDTH - 0.5) * 2);
	m_ty2.push_back((m_y2[0] / SCREEN_HEIGHT - 0.5) * -2);




	// set up vertex data (and buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	float vertices[16] = {
		// positions    // texture coords
		 m_tx2[0], m_ty1[0],   1.0f, 1.0f,   // top right
		 m_tx2[0], m_ty2[0],   1.0f, 0.0f,   // bottom right
		 m_tx1[0], m_ty2[0],   0.0f, 0.0f,   // bottom left
		 m_tx1[0], m_ty1[0],   0.0f, 1.0f    // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (m_mobility == mobility::moveable || m_mobility == mobility::camera_independent) glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


}

image::~image() {
	println("IMAGE DESTROYEED");

	//delete(m_colorData);
	for(int i = 0; i < m_texture_count; i++) glDeleteTextures(1, &m_textureID[i]);

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void image::draw() {
	//bind textures to shaders
	for (uint8_t i = 0; i < m_texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureID[i]);
	}

	m_shader.use();
	//set uniforms here
	//glUniform3f(glGetUniformLocation(m_shader.m_ID, "iCam"), cam.m_camera_center[0], cam.m_camera_center[1], cam.m_camera_zoom);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indecesNum, GL_UNSIGNED_INT, 0);

}

void image::move(int x, int y, int object) {
	if (m_mobility == mobility::moveable ||
		m_mobility == mobility::camera_independent) {

		if (!m_drag[object]) {
			m_x1[object] += x;
			m_y1[object] += y;
			m_x2[object] += x;
			m_y2[object] += y;

			float t_x_gain =  x / HALF_SCREEN_WIDTH;
			float t_y_gain = -y / HALF_SCREEN_HEIGHT;
			m_tx1[object] += t_x_gain;
			m_ty1[object] += t_y_gain;
			m_tx2[object] += t_x_gain;
			m_ty2[object] += t_y_gain;

			int index = object * m_vertices_count;
			if (m_vertices_count == 20) {
				m_vertices[index] += t_x_gain;
				m_vertices[index + 1] += t_y_gain;
				//m_vertices[index + 2] = 1.0f;
				//m_vertices[index + 3] = 1.0f;
				//m_vertices[index + 4] = texture;

				m_vertices[index + 5] += t_x_gain;
				m_vertices[index + 6] += t_y_gain;
				//m_vertices[index + 7] = 1.0f;
				//vertices[index + 8] = 0.0f;
				//vertices[index + 9] = texture;

				m_vertices[index + 10] += t_x_gain;
				m_vertices[index + 11] += t_y_gain;
				//vertices[index + 12] = 0.0f;
				//vertices[index + 13] = 0.0f;
				//vertices[index + 14] = texture;

				m_vertices[index + 15] += t_x_gain;
				m_vertices[index + 16] += t_y_gain;
				//vertices[index + 17] = 0.0f;
				//m_vertices[index + 18] = 1.0f;
				//m_vertices[index + 19] = texture;
			}
			else if (m_vertices_count == 16) {
				m_vertices[index] += t_x_gain;
				m_vertices[index + 1] += t_y_gain;
				//m_vertices[index + 2] = 1.0f;
				//m_vertices[index + 3] = 1.0f;

				m_vertices[index + 4] += t_x_gain;
				m_vertices[index + 5] += t_y_gain;
				//m_vertices[index + 6] = 1.0f;
				//vertices[index + 7] = 0.0f;

				m_vertices[index + 8] += t_x_gain;
				m_vertices[index + 9] += t_y_gain;
				//vertices[index + 10] = 0.0f;
				//vertices[index + 11] = 0.0f;

				m_vertices[index + 12] += t_x_gain;
				m_vertices[index + 13] += t_y_gain;
				//vertices[index + 14] = 0.0f;
				//m_vertices[index + 15] = 1.0f;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //is not needed and can't really find out why. would think it is needed to bind buffer before using glbuffersubdata
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), &m_vertices[0]);
		}
	}
}
void image::set_position(int x, int y, bool center, int object) {
	if (m_mobility == mobility::moveable ||
		m_mobility == mobility::camera_independent) {
		if (center) {
			m_x1[object] = x - m_width / 2.0f;
			m_y1[object] = y - m_height / 2.0f;
			m_tx1[object] = (m_x1[object] / SCREEN_WIDTH - 0.5f) * 2.0f; //* cam.m_reciprocal_zoom;
			m_ty1[object] = (m_y1[object] / SCREEN_HEIGHT - 0.5f) * -2.0f; //*cam.m_reciprocal_zoom;
		}
		else {
			m_x1[object] = x;
			m_y1[object] = y;
			m_tx1[object] = (x / SCREEN_WIDTH - 0.5f) * 2.0f;
			m_ty1[object] = (y / SCREEN_HEIGHT - 0.5f) * -2.0f;
		}
		m_original_positions[object][0] = m_x1[object];
		m_original_positions[object][1] = m_y1[object];


		m_x2[object] = m_x1[object] + m_width;
		m_y2[object] = m_y1[object] + m_height;
		m_tx2[object] = (m_x2[object] / SCREEN_WIDTH - 0.5f) * 2.0f;
		m_ty2[object] = (m_y2[object] / SCREEN_HEIGHT - 0.5f) * -2.0f;


		int index = object * m_vertices_count;
		if (m_vertices_count == 20) {
			m_vertices[index] = m_tx2[object];
			m_vertices[index + 1] = m_ty1[object];
			//m_vertices[index + 2] = 1.0f;
			//m_vertices[index + 3] = 1.0f;
			//m_vertices[index + 4] = texture;

			m_vertices[index + 5] = m_tx2[object];
			m_vertices[index + 6] = m_ty2[object];
			//m_vertices[index + 7] = 1.0f;
			//vertices[index + 8] = 0.0f;
			//vertices[index + 9] = texture;

			m_vertices[index + 10] = m_tx1[object];
			m_vertices[index + 11] = m_ty2[object];
			//vertices[index + 12] = 0.0f;
			//vertices[index + 13] = 0.0f;
			//vertices[index + 14] = texture;

			m_vertices[index + 15] = m_tx1[object];
			m_vertices[index + 16] = m_ty1[object];
			//vertices[index + 17] = 0.0f;
			//m_vertices[index + 18] = 1.0f;
			//m_vertices[index + 19] = texture;
		}
		else if (m_vertices_count == 16) {
			m_vertices[index] = m_tx2[object];
			m_vertices[index + 1] = m_ty1[object];
			//m_vertices[index + 2] = 1.0f;
			//m_vertices[index + 3] = 1.0f;

			m_vertices[index + 4] = m_tx2[object];
			m_vertices[index + 5] = m_ty2[object];
			//m_vertices[index + 6] = 1.0f;
			//vertices[index + 7] = 0.0f;

			m_vertices[index + 8] = m_tx1[object];
			m_vertices[index + 9] = m_ty2[object];
			//vertices[index + 10] = 0.0f;
			//vertices[index + 11] = 0.0f;

			m_vertices[index + 12] = m_tx1[object];
			m_vertices[index + 13] = m_ty1[object];
			//vertices[index + 14] = 0.0f;
			//m_vertices[index + 15] = 1.0f;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //is not needed and can't really find out why. would think it is needed to bind buffer before using glbuffersubdata
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), &m_vertices[0]);
	}
}

void image::hover(void (*on_hover_function)(image*, bool, int), bool toggle) {
	if (m_mobility == mobility::camera_independent ||
		m_mobility == mobility::stationary) {
		for (int i = 0; i < m_object_count; i++) {
			if (MOUSE_X > m_x1[i] && MOUSE_X < m_x2[i] &&
				MOUSE_Y > m_y1[i] && MOUSE_Y < m_y2[i]) {
				if (toggle) {
					if (!m_hover_toggle[i]) {
						m_hover_toggle[i] = true;
						on_hover_function(this, true, i);
					}
				}
			}
			else if (toggle) {
				if (m_hover_toggle[i]) {
					m_hover_toggle[i] = false;
					on_hover_function(this, false, i);
				}
			}
		}
	}
	else {
		float m_x = MOUSE_T_X / cam.m_camera_zoom - cam.m_t_camera_offset[0];
		float m_y = MOUSE_T_Y / cam.m_camera_zoom - cam.m_t_camera_offset[1];
		for (int i = 0; i < m_object_count; i++) {
			if (m_x > m_tx1[i] &&
				m_x < m_tx2[i] &&
				m_y < m_ty1[i] &&
				m_y > m_ty2[i] ) {
			/*if (MOUSE_T_X > (m_tx1[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
				MOUSE_T_X < (m_tx2[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
				MOUSE_T_Y < (m_ty1[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom &&
				MOUSE_T_Y > (m_ty2[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom) {*/

				if (toggle) {
					if (!m_hover_toggle[i]) {
						m_hover_toggle[i] = true;
						on_hover_function(this, true, i);
					}
				}
			}
			else if (toggle) {
				if (m_hover_toggle[i]) {
					m_hover_toggle[i] = false;
					on_hover_function(this, false, i);
				}
			}
		}

	}
}
void image::click(void (*on_click_function)(image*, int), bool toggle) {
	if (m_mobility == mobility::camera_independent ||
		m_mobility == mobility::stationary) {
		for (int i = 0; i < m_object_count; i++) {
			if (MOUSE_LEFT_LAST_PRESS_X > m_x1[i] && MOUSE_LEFT_LAST_PRESS_X < m_x2[i] &&
				MOUSE_LEFT_LAST_PRESS_Y > m_y1[i] && MOUSE_LEFT_LAST_PRESS_Y < m_y2[i]) {
				if(MOUSE_LEFT_PRESS) {

					if (toggle) {
						if (!m_click_toggle[i]) {
							m_click_toggle[i] = true;
							on_click_function(this, i);
						}
					}
				}
				else if (toggle) {
					if (m_click_toggle[i]) m_click_toggle[i] = false;
				}
			}
		}
	}
	else {
		float m_x = MOUSE_T_X / cam.m_camera_zoom - cam.m_t_camera_offset[0];
		float m_y = MOUSE_T_Y / cam.m_camera_zoom - cam.m_t_camera_offset[1];
		for (int i = 0; i < m_object_count; i++) {
			/*if (MOUSE_T_X > (m_tx1[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
				MOUSE_T_X < (m_tx2[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
				MOUSE_T_Y < (m_ty1[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom &&
				MOUSE_T_Y > (m_ty2[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom) {*/
			if (m_x > m_tx1[i] &&
				m_x < m_tx2[i] &&
				m_y < m_ty1[i] &&
				m_y > m_ty2[i]) {
				if (MOUSE_LEFT_PRESS) {

					if (toggle) {
						if (!m_click_toggle[i]) {
							m_click_toggle[i] = true;
							on_click_function(this, i);
						}
					}
				}
				else if (toggle) {
					if (m_click_toggle[i]) m_click_toggle[i] = false;
				}
			}
		}
	}
}
bool image::drag(void (*on_drag_end_function)(image*, int, std::vector<int>), bool center) {
	if (m_mobility == mobility::camera_independent ||
		m_mobility == mobility::stationary) {
		if (MOUSE_LEFT_PRESS) {
			for (int i = 0; i < m_object_count; i++) {
				if (MOUSE_LEFT_LAST_PRESS_X > m_x1[i] && MOUSE_LEFT_LAST_PRESS_X < m_x2[i] &&
					MOUSE_LEFT_LAST_PRESS_Y > m_y1[i] && MOUSE_LEFT_LAST_PRESS_Y < m_y2[i]) {
					if (!m_drag[i]) {

						m_drag[i] = true;
						m_original_positions[i] = { m_x1[i], m_y1[i] };
						m_saved_mouse_position[0] = MOUSE_X;
						m_saved_mouse_position[1] = MOUSE_Y;
					}
				}
				if (m_drag[i]) {

					if(center) set_position(MOUSE_X, MOUSE_Y, true, i);
					else set_position(MOUSE_X - (m_saved_mouse_position[0] - m_original_positions[i][0]), 
									  MOUSE_Y - (m_saved_mouse_position[1] - m_original_positions[i][1]), false, i);
				
				}
			}
		}
		else {
			for (int i = 0; i < m_object_count; i++) {
				if (m_drag[i]) {
					m_drag[i] = false;
					on_drag_end_function(this, i, m_original_positions[i]); //reset position with m_original_positions or send the image somewhere else
					return true;
				}
			}
		}
	}
	else {
		if (MOUSE_LEFT_PRESS) {
			for (int i = 0; i < m_object_count; i++) {
				if (MOUSE_T_X > (m_tx1[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
					MOUSE_T_X < (m_tx2[i] + cam.m_t_camera_offset[0]) * cam.m_camera_zoom &&
					MOUSE_T_Y < (m_ty1[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom &&
					MOUSE_T_Y >(m_ty2[i] + cam.m_t_camera_offset[1]) * cam.m_camera_zoom) {

					if (!m_drag[i]) {
						m_drag[i] = true;
						m_original_positions[i] = { m_x1[i], m_y1[i] };
						m_saved_mouse_position[0] = MOUSE_X;
						m_saved_mouse_position[1] = MOUSE_Y;
					}
				}

				if (m_drag[i]) {
					if (center) set_position(MOUSE_X, MOUSE_Y, true, i);
					else set_position(MOUSE_X - (m_saved_mouse_position[0] - m_original_positions[i][0]),
						MOUSE_Y - (m_saved_mouse_position[1] - m_original_positions[i][1]), true, i);
				}
			}
		}
		else {
			for (int i = 0; i < m_object_count; i++) {
				if (m_drag[i]) {
					m_drag[i] = false;
					on_drag_end_function(this, i, m_original_positions[i]); //reset position with m_original_positions or send the image somewhere else
					return true;
				}
			}
		}
	}
	return false;
}

void image::switch_texture(std::vector<float> texture_indexes) {
	if (texture_indexes.size() == m_texture_indexes.size()) {
		for (int i = 0; i < texture_indexes.size(); i++) m_texture_indexes[i] = texture_indexes[i];

		for (int i = 0; i < m_object_count; i++) {
			int index = i * 20;

			if (m_vertices[index + 4] != texture_indexes[i]) {
				m_vertices[index + 4] = texture_indexes[i];
				m_vertices[index + 9] = texture_indexes[i];
				m_vertices[index + 14] = texture_indexes[i];
				m_vertices[index + 19] = texture_indexes[i];
			}

			/*
			m_vertices[index + 4] = texture_indexes[i];
			m_vertices[index + 9] = texture_indexes[i];
			m_vertices[index + 14] = texture_indexes[i];
			m_vertices[index + 19] = texture_indexes[i];
			*/
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //is not needed and can't really find out why. would think it is needed to bind buffer before using glbuffersubdata
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), &m_vertices[0]);
	}
}