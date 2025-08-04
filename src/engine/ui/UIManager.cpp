#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include "../../game/CommandModule.h"


UIManager::UIManager()
    : m_TextRenderer(nullptr), m_CommandModule(nullptr), m_ScreenWidth(0), m_ScreenHeight(0)
{
    m_PanelColor = glm::vec4(0.1f, 0.15f, 0.2f, 0.8f);
    m_TextColor = glm::vec3(0.8f, 0.85f, 0.9f);
    m_HighlightColor = glm::vec3(0.5f, 0.8f, 1.0f);
}

UIManager::~UIManager() {
    delete m_TextRenderer;
}

void UIManager::Init(unsigned int screenWidth, unsigned int screenHeight, CommandModule* cmdModule) {
    m_CommandModule = cmdModule;
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;

    m_TextRenderer = new TextRenderer(screenWidth, screenHeight);
    if (!m_TextRenderer->Load("assets/fonts/eurostile.TTF", 16)) {
        std::cerr << "Failed to load UI font." << std::endl;
        return;
    }

    AddButton("SYSTEM POWER", 20, 40, 150, 30, [this]() {
        if(m_CommandModule) m_CommandModule->TogglePower();
    });
    AddButton("WIREFRAME", 180, 40, 150, 30, []() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    });
    AddButton("SOLID", 340, 40, 150, 30, []() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    });

    AddLabel("TELEMETRY", 20, 100, false);
    AddLabel("CAM POS:", 20, 125, true, nullptr);
    AddLabel("CAM DIR:", 20, 145, true, nullptr);
    // FIX: Ensure both return paths have the same type (std::string)
    AddLabel("STATUS:", 20, 180, true, [this](){
        if(m_CommandModule) return "POWER: " + std::string(m_CommandModule->IsPowered() ? "ONLINE" : "OFFLINE");
        return std::string("POWER: N/A");
    });
}

void UIManager::Update(const Camera& camera) {
    for (auto& label : m_Labels) {
        if (label.isDynamic) {
            if (label.text.rfind("CAM POS:", 0) == 0) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << "CAM POS: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z;
                label.updateFunc = [ss_str = ss.str()](){ return ss_str; };
            }
            if (label.text.rfind("CAM DIR:", 0) == 0) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << "CAM DIR: " << camera.Front.x << ", " << camera.Front.y << ", " << camera.Front.z;
                label.updateFunc = [ss_str = ss.str()](){ return ss_str; };
            }
        }
    }
}


void UIManager::Render() {
    if (!m_TextRenderer) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    RenderPanel(10, 10, 500, 70, m_PanelColor);
    RenderPanel(10, 90, 250, 120, m_PanelColor);

    for (const auto& button : m_Buttons) {
        glm::vec3 color = button.isHovered ? m_HighlightColor : m_TextColor;
        float renderY = m_ScreenHeight - button.y - button.height / 2; // Centered
        m_TextRenderer->RenderText(button.label, button.x + 10, renderY, 1.0f, color);
    }
    
    for (auto& label : m_Labels) {
        std::string textToRender = label.text;
        if(label.isDynamic && label.updateFunc){
            textToRender = label.updateFunc();
        }
        float renderY = m_ScreenHeight - label.y;
        m_TextRenderer->RenderText(textToRender, label.x, renderY, label.scale, label.color);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void UIManager::RenderPanel(float x, float y, float width, float height, const glm::vec4& color) {
    std::string block_char = "█";
    glm::ivec2 char_size = m_TextRenderer->GetCharacterSize(block_char[0]);

    if(char_size.x == 0) return;

    float scale = width / char_size.x;
    glm::vec3 quadColor = glm::vec3(color.r, color.g, color.b);
    float renderY = m_ScreenHeight - y;

    for(float h = 0; h < height; h += char_size.y * (scale/width) * 0.5f){
        m_TextRenderer->RenderText(block_char, x, renderY - h, scale, quadColor);
    }
}


void UIManager::HandleClick(float x, float y) {
    for (auto& button : m_Buttons) {
        if (x >= button.x && x <= button.x + button.width && y >= button.y && y <= button.y + button.height) {
            if (button.onClick) button.onClick();
        }
    }
}

void UIManager::HandleMouseMove(float x, float y) {
    for (auto& button : m_Buttons) {
        button.isHovered = (x >= button.x && x <= button.x + button.width && y >= button.y && y <= button.y + button.height);
    }
}

void UIManager::OnWindowResize(unsigned int newWidth, unsigned int newHeight) {
    m_ScreenWidth = newWidth;
    m_ScreenHeight = newHeight;
    if (m_TextRenderer) {
        m_TextRenderer->UpdateProjection(newWidth, newHeight);
    }
}


void UIManager::AddButton(std::string label, float x, float y, float width, float height, std::function<void()> onClick) {
    UIButton button = {label, x, y, width, height, onClick};
    m_Buttons.push_back(button);
}

void UIManager::AddLabel(std::string text, float x, float y, bool isDynamic, std::function<std::string()> updateFunc) {
    UILabel label = {text, x, y, 1.0f, m_TextColor, isDynamic, updateFunc};
    m_Labels.push_back(label);
}