#pragma once
#include "common_includes.h"
#include "simple_shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#define ft_pixel_mode_bgra   FT_PIXEL_MODE_BGRA


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    int Size[2];             // Size of glyph
    int Bearing[2];          // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class text_database {
public:
	GLuint m_VAO, m_VBO, m_EBO;
	simple_shader m_shader;
	unsigned int m_font_height;
	 
    std::map<char, Character> Characters;
	text_database();
	text_database(std::string font_type, int font_size);
	~text_database();

	void render_text(std::string text, float x, float y, float scale, float color[4], float space = 0.02f);

private:
	std::string m_font_type;
	const char* text_vertex_shader = R"HERE(
		#version 330 core
		in vec4 aVertex;
		
		out vec2 TexCoords;

		uniform mat4 projection;

		void main()
		{
			gl_Position = vec4(aVertex.xy, 0.0, 1.0);
			TexCoords = aVertex.zw;
		})HERE";

	const char* text_fragment_shader = R"HERE(
		#version 330 core
		in vec2 TexCoords;
		out vec4 color;

		uniform sampler2D text;
		uniform vec4 textColor;

		void main()
		{    
			color = vec4(textColor.xyz, texture(text, TexCoords).r * textColor.w);
		})HERE";
	unsigned int m_indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
};