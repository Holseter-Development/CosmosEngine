#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>
#include <string>
#include <glm/glm.hpp>

#include "../graphics/Shader.h"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool Init(const char* fontPath, unsigned int fontSize, unsigned int screenWidth, unsigned int screenHeight);
    void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);

private:
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    Shader* m_Shader;
    glm::mat4 m_Projection;
};

#endif
