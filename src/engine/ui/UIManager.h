// src/engine/ui/UIManager.h
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <vector>
#include <functional>
#include <string>
#include "TextRenderer.h"
#include "../graphics/Camera.h"

// --- UI Components ---
struct UIButton {
    std::string label;
    float x, y, width, height;
    std::function<void()> onClick;
    bool isHovered = false;
};

struct UILabel {
    std::string text;
    float x, y;
    float scale = 1.0f;
    glm::vec3 color = glm::vec3(0.8f, 0.85f, 0.9f);
    bool isDynamic = false;
    std::function<std::string()> updateFunc;
};

// FIX: Forward-declare CommandModule to resolve the identifier error
class CommandModule;

class UIManager {
public:
    UIManager();
    ~UIManager();

    void Init(unsigned int screenWidth, unsigned int screenHeight, CommandModule* cmdModule);
    void Update(const Camera& camera); 
    void Render();
    
    // Input Handlers
    void HandleClick(float x, float y);
    void HandleMouseMove(float x, float y);
    void OnWindowResize(unsigned int newWidth, unsigned int newHeight);

private:
    // UI Rendering
    void RenderPanel(float x, float y, float width, float height, const glm::vec4& color);
    void AddButton(std::string label, float x, float y, float width, float height, std::function<void()> onClick);
    void AddLabel(std::string text, float x, float y, bool isDynamic = false, std::function<std::string()> updateFunc = nullptr);
    
    // State
    TextRenderer* m_TextRenderer;
    CommandModule* m_CommandModule; // This is now correctly recognized
    unsigned int m_ScreenWidth;
    unsigned int m_ScreenHeight;
    
    // UI Elements
    std::vector<UIButton> m_Buttons;
    std::vector<UILabel> m_Labels;

    // Theming
    glm::vec4 m_PanelColor;
    glm::vec3 m_TextColor;
    glm::vec3 m_HighlightColor;
};

#endif