#include "UIManager.h"
#include "../graphics/Shader.h"
#include "../../game/CommandModule.h"
#include <glm/vec3.hpp>

UIManager::UIManager() : m_CommandModule(nullptr) {}

void UIButton::Draw(TextRenderer& renderer) const {
    renderer.RenderText(Label, X, Y, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

bool UIButton::Contains(float x, float y) const {
    return x >= X && x <= X + Width && y >= Y && y <= Y + Height;
}

bool UIManager::Init(unsigned int screenWidth, unsigned int screenHeight, CommandModule* cmdModule) {
    m_CommandModule = cmdModule;
    if (!m_TextRenderer.Init("assets/fonts/eurostile.TTF", 24, screenWidth, screenHeight))
        return false;

    UIButton powerBtn;
    powerBtn.Label = "TOGGLE POWER";
    powerBtn.X = 20.0f;
    powerBtn.Y = screenHeight - 40.0f;
    powerBtn.Width = 200.0f;
    powerBtn.Height = 30.0f;
    powerBtn.OnClick = [this]() { TogglePower(); };

    m_Buttons.push_back(powerBtn);
    return true;
}

void UIManager::Render() {
    for (const auto& btn : m_Buttons) {
        btn.Draw(m_TextRenderer);
    }
    if (m_CommandModule) {
        std::string status = "POWER: ";
        status += m_CommandModule->IsPowered() ? "ON" : "OFF";
        m_TextRenderer.RenderText(status, 20.0f, 40.0f, 1.0f, glm::vec3(1.0f));
    }
}

void UIManager::HandleClick(float x, float y) {
    for (auto& btn : m_Buttons) {
        if (btn.Contains(x, y) && btn.OnClick) {
            btn.OnClick();
        }
    }
}

void UIManager::TogglePower() {
    if (m_CommandModule) {
        m_CommandModule->TogglePower();
    }
}
