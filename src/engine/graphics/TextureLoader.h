#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <vector>
#include <string>

// Forward declare Assimp type to avoid including its headers here
struct aiTexture;

class TextureLoader {
public:
    // Load texture from a file path
    static unsigned int loadTexture(const char* path);
    // Load texture from embedded data in memory
    static unsigned int loadTextureFromMemory(const aiTexture* pTexture);
    // Load a cubemap from file paths
    static unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif