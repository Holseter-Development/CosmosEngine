#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <vector>
#include <functional>
#include "TextRenderer.h"

class UIButton {
public:
    std::string Label;
    float X, Y, Width, Height;
    std::function<void()> OnClick;

    void Draw(TextRenderer& renderer) const;
    bool Contains(float x, float y) const;
};

class CommandModule;

class UIManager {
public:
    UIManager();
    bool Init(unsigned int screenWidth, unsigned int screenHeight, CommandModule* cmdModule);
    void Render();
    void HandleClick(float x, float y);
    void TogglePower();
private:
    TextRenderer m_TextRenderer;
    std::vector<UIButton> m_Buttons;
    CommandModule* m_CommandModule;
};

#endif
