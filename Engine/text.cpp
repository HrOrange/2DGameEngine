#include "text.h"
#include "global_variables.h"


text::text(text_database* my_text_database, std::string my_text, int x, int y, float color[4], float scale, float space, unsigned int vertical_align)
	: m_database(my_text_database),
	  //m_text(my_text), //this variable is getting initialized on line 13 by set_text(my_text)
	  m_x(x),
	  m_y(y),
      m_scale(scale),
      m_space(space),
      m_vertical_align(vertical_align)
{
    for (int i = 0; i < 4; i++) m_color[i] = color[i] / 255.0f;
    
    if(my_text.length() > 0) set_text(my_text);
    m_vertices_size = sizeof(float) * 16; //sizeof(float) == 4, og grunden til 16 er antallet af elementer i hvert m_vertices element
    //(x / window->m_width - 0.5f) * 2

    //glUniform4f(glGetUniformLocation(m_database->m_shader.m_ID, "textColor"), m_color[0], m_color[1], m_color[2]);

    m_database->m_shader.use();

    glUniform4f(glGetUniformLocation(m_database->m_shader.m_ID, "textColor"), m_color[0], m_color[1], m_color[2], m_color[3]);

}

text::~text() {
    for (int i = 0; i < m_vertices.size(); i++) delete[] m_vertices[i];
}

void text::draw() {
	m_database->m_shader.use();

    glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_database->m_VAO);

    const char* text_char = m_text.c_str();
    for (int i = 0; i < m_text.length(); i++)
    {
        Character ch = m_database->Characters[text_char[i]];
        
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_database->m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices[i]);


        // render quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void text::set_text(std::string new_text) {
    m_previous_text = m_text;
    m_text = new_text;
    
    m_tx.resize(m_text.length());
    m_ty.resize(m_text.length());
    
    const char* text_char = m_text.c_str();



    //first set the first value
    Character ch = m_database->Characters[text_char[0]];
    
    m_tx[0] = (m_x / SCREEN_WIDTH - 0.5f) * 2.0f;


    /*if (i > 0) m_tx[i] = m_tx[i - 1] + (ch.Advance / 64.0f) / HALF_SCREEN_WIDTH + m_space * m_scale;
    else m_tx[i] = (ch.Advance / 64.0f) / HALF_SCREEN_WIDTH;*/
    //if (go_through_list(parenthesis, &(text_char[i]))) m_ty[i] = ((m_y + m_database->m_font_height - ch.Size[1]) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    //else m_ty[i] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    if (m_vertical_align == 0) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    else if (m_vertical_align == 1) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    else if (m_vertical_align == 2) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    
    bool there = false;
    for (int j = 0; j < sizeof(parenthesis) / sizeof(parenthesis[0]); j++) {
        if (text_char[0] == *parenthesis[j]) {
            if (m_vertical_align == 0) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1]) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
            else if (m_vertical_align == 1) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1]) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
            //else if (m_vertical_align == 2) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1]) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f; //i chose to set parenthesis' in the middle of the text in vertical align 2
            else if (m_vertical_align == 2) m_ty[0] = ((m_y + (m_database->m_font_height - ch.Size[1]) * 2.0f) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
            
            there = true;
            break;
        }
    }
    if (!there) {
        if (m_vertical_align == 0) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
        else if (m_vertical_align == 1) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
        else if (m_vertical_align == 2) m_ty[0] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
    }



    float w = (ch.Size[0] * m_scale) / HALF_SCREEN_WIDTH;
    float h = (ch.Size[1] * m_scale) / HALF_SCREEN_HEIGHT;
    
    m_vertices = { new float[16]{
        //positions                 // texture coords
        m_tx[0] + w,  m_ty[0],       1.0f, 0.0f,   // top right
        m_tx[0] + w,  m_ty[0] - h,   1.0f, 1.0f,   // bottom right
        m_tx[0],      m_ty[0] - h,   0.0f, 1.0f,   // bottom left
        m_tx[0],      m_ty[0],       0.0f, 0.0f    // top left 
    }};


    if (m_vertical_align == 0) {
        for (int i = 1; i < m_text.length(); i++)
        { 
            Character ch = m_database->Characters[text_char[i]];


            m_tx[i] = m_tx[i - 1] + (m_database->Characters[text_char[i - 1]].Advance / 64.0f) / HALF_SCREEN_WIDTH + m_space * m_scale;
            

            there = false;
            for (int j = 0; j < sizeof(parenthesis) / sizeof(parenthesis[0]); j++) {
                if (text_char[i] == *parenthesis[j]) {
                    m_ty[i] = (m_y * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
                    there = true;
                    break;
                }
            }
            if (!there) m_ty[i] = ((m_y + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
            
            float w = (ch.Size[0] * m_scale) / HALF_SCREEN_WIDTH;
            float h = (ch.Size[1] * m_scale) / HALF_SCREEN_WIDTH;

            m_vertices.push_back(new float[16]{
                //positions    // texture coords
                  m_tx[i] + w,  m_ty[i],       1.0f, 0.0f,   // top right
                  m_tx[i] + w,  m_ty[i] - h,   1.0f, 1.0f,   // bottom right
                  m_tx[i],      m_ty[i] - h,   0.0f, 1.0f,   // bottom left
                  m_tx[i],      m_ty[i],       0.0f, 0.0f    // top left 
                });

        }
    }
    else if (m_vertical_align == 1) {
        for (int i = 1; i < m_text.length(); i++)
        {
            Character ch = m_database->Characters[text_char[i]];

            m_tx[i] = m_tx[i - 1] + (m_database->Characters[text_char[i - 1]].Advance / 64.0f) / HALF_SCREEN_WIDTH + m_space * m_scale;
            
            there = false;
            for (int j = 0; j < sizeof(parenthesis) / sizeof(parenthesis[0]); j++) {
                if (text_char[i] == *parenthesis[j]) {
                    m_ty[i] = ((m_y + (m_database->m_font_height - ch.Size[1]) / 2) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;
                    there = true;
                    break;
                }
            }
            if (!there) m_ty[i] = ((m_y + (m_database->m_font_height - ch.Size[1]) / 2 + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;


            float w = (ch.Size[0] * m_scale) / HALF_SCREEN_WIDTH;
            float h = (ch.Size[1] * m_scale) / HALF_SCREEN_WIDTH;

            m_vertices.push_back(new float[16]{
                //positions    // texture coords
                  m_tx[i] + w,  m_ty[i],       1.0f, 0.0f,   // top right
                  m_tx[i] + w,  m_ty[i] - h,   1.0f, 1.0f,   // bottom right
                  m_tx[i],      m_ty[i] - h,   0.0f, 1.0f,   // bottom left
                  m_tx[i],      m_ty[i],       0.0f, 0.0f    // top left 
                });

        }
    }
    else if (m_vertical_align == 2) {
        for (int i = 1; i < m_text.length(); i++)
        {
            Character ch = m_database->Characters[text_char[i]];

            m_tx[i] = m_tx[i - 1] + (m_database->Characters[text_char[i - 1]].Advance / 64.0f) / HALF_SCREEN_WIDTH + m_space * m_scale;
            
            //if(*(parenthesis[0]) == text_char[0])
            there = false;
            for (int j = 0; j < sizeof(parenthesis) / sizeof(parenthesis[0]); j++) {
                if (text_char[i] == *parenthesis[j]) {
                    //m_ty[i] = ((m_y + m_database->m_font_height - ch.Size[1]) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f; old normal
                    m_ty[i] = ((m_y + (m_database->m_font_height - ch.Size[1]) * 2.0f) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;

                    there = true;
                    break;
                }
            }
            if(!there) m_ty[i] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;

            //m_ty[i] = ((m_y + m_database->m_font_height - ch.Size[1] + (ch.Size[1] - ch.Bearing[1])) * m_scale / SCREEN_HEIGHT - 0.5f) * -2.0f;

            float w = (ch.Size[0] * m_scale) / HALF_SCREEN_WIDTH;
            float h = (ch.Size[1] * m_scale) / HALF_SCREEN_HEIGHT;

            m_vertices.push_back(new float[16]{
              //positions                 // texture coords
                m_tx[i] + w,  m_ty[i],       1.0f, 0.0f,   // top right
                m_tx[i] + w,  m_ty[i] - h,   1.0f, 1.0f,   // bottom right
                m_tx[i],      m_ty[i] - h,   0.0f, 1.0f,   // bottom left
                m_tx[i],      m_ty[i],       0.0f, 0.0f    // top left 
            });

        }
    }
}