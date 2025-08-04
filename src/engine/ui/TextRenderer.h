// src/engine/ui/TextRenderer.h
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../graphics/Shader.h"

// Holds all state information relevant to a character as loaded using Freetype
struct Character {
    unsigned int TextureID; 
    glm::ivec2   Size;      
    glm::ivec2   Bearing;   
    unsigned int Advance;   
};

class TextRenderer {
public:
    TextRenderer(unsigned int screenWidth, unsigned int screenHeight);
    ~TextRenderer();
    bool Load(std::string font, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
    void UpdateProjection(unsigned int screenWidth, unsigned int screenHeight);

    // FIX: Add a public method to safely get character info
    glm::ivec2 GetCharacterSize(char character);

private:
    Shader TextShader;
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
};

#endif