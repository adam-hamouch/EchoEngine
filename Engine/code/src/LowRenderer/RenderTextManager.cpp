#include "LowRenderer/RenderTextManager.h"

#include "Core/Engine.h"

#include "Resources/IShader.h"
#include "Resources/ResourceManager.h"

#include <ft2build.h>

#include "Core/UI/UI.h"

#include FT_FREETYPE_H

void TextBuffer::Init()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}

void TextBuffer::UnbindBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TextVertexAttributes::Bind()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void TextVertexAttributes::Init()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextBuffer::~TextBuffer()
{
    glDeleteBuffers(1, &VBO);
}

TextVertexAttributes::~TextVertexAttributes()
{
    glDeleteVertexArrays(1, &VAO);
}

void RenderTextManager::Render(std::string text, float xPercent, float yPercent, float scale, Vector3D color)
{
    int screenWidth = Engine::GetInstance()->GetUI()->GetWidthScene();
    int screenHeight = Engine::GetInstance()->GetUI()->GetHeightScene();

    float x = xPercent * screenWidth;
    float y = yPercent * screenHeight;

    IShader* shader = Engine::GetInstance()->GetResourceManager()->Get<IShader>("text_shader.json");

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shader->GetProgram());
    glUniform3f(glGetUniformLocation(shader->GetProgram(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertexAttributes.VAO);

    for (char c : text)
    {
        Character ch = characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
}


void RenderTextManager::LoadFont(std::string path)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return; 
    }

    std::string font_name = path; 
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return; 
    }

    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return; 
    }
    else {
        FT_Set_Pixel_Sizes(face, 0, 96);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

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

        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        	Character character = {
                texture,
                Vector2D( static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
                Vector2D(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

	FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderTextManager::LoadBuffer()
{
    vertexAttributes.Bind();
    buffer.Init();
    vertexAttributes.Init();
    buffer.UnbindBuffer();
}

void RenderTextManager::SetProjection(const Mat4& projection) const
{
    IShader* shader = Engine::GetInstance()->GetResourceManager()->Get<IShader>("text_shader.json");

    if (!shader)
        return; 

    glUseProgram(shader->GetProgram());
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, projection.data); 
}
