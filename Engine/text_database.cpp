#include "text_database.h"
#include "global_variables.h"

text_database::text_database() {

}
text_database::text_database(std::string font_type, int font_size)
    : m_font_height(font_size),
      m_font_type(font_type)
{
    
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    else {
        FT_Face face;
        if (FT_New_Face(ft, ("fonts/" + m_font_type + ".ttf").c_str(), 0, &face)) std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Set_Pixel_Sizes(face, 0, font_size);
        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;



        //now add all the 128 ASCII characters to the Characters map

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            //println(face->glyph->bitmap);

            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                { face->glyph->bitmap.width, face->glyph->bitmap.rows },
                { face->glyph->bitmap_left, face->glyph->bitmap_top },
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
    }
    FT_Done_FreeType(ft);

    //Setup the VAO and VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    

    //this needs to be fixed
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //setup shader
    m_shader.init(text_vertex_shader, text_fragment_shader);
}

text_database::~text_database() {

}

void text_database::render_text(std::string text, float x, float y, float scale, float color[4], float space)
{
    //x = (x / window->m_width - 0.5) * -2;
    //y = (y / window->m_height - 0.5) * -2;

    // activate corresponding render state	
    m_shader.use();
    glUniform4f(glGetUniformLocation(m_shader.m_ID, "textColor"), color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    
   
    // iterate through all characters
    float xpos = (x / SCREEN_WIDTH - 0.5f) * 2.0f;
    const char* text_char = text.c_str();

    for (int i = 0; i < text.length(); i++)
    {
        Character ch = Characters[text_char[i]];

        //if (*c != text[0]) xpos += 1.0f / window->m_width * 14;
        
        float ypos = ((y + (m_font_height - ch.Size[1]) / 2) * scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
        
        float w = (ch.Size[0] * scale) / HALF_SCREEN_WIDTH;
        float h = (ch.Size[1] * scale) / HALF_SCREEN_HEIGHT;

        // update VBO for each character
        float m_vertices[16] = {
            // positions    // texture coords
             xpos + w,  ypos,       1.0f, 0.0f,   // top right
             xpos + w,  ypos - h,   1.0f, 1.0f,   // bottom right
             xpos,      ypos - h,   0.0f, 1.0f,   // bottom left
             xpos,      ypos,       0.0f, 0.0f    // top left 
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);

        
        // render quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        xpos += (ch.Advance / 64.0f) / HALF_SCREEN_WIDTH + space * scale;
    }
}