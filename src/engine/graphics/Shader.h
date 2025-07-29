#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp> // Make sure to include glm here

class Shader {
public:
    // The program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // Use/activate the shader
    void use();

    // Utility uniform functions
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, float v1, float v2, float v3) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setInt(const std::string &name, int value) const;

private:
    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif