// src/engine/core/InputManager.cpp
#include "InputManager.h"
#include <glad/glad.h>
#include "../ui/UIManager.h"

// Initialize static variables
Camera* InputManager::s_Camera = nullptr;
UIManager* InputManager::s_UIManager = nullptr;
float InputManager::s_LastX = 0.0f;
float InputManager::s_LastY = 0.0f;
bool InputManager::s_FirstMouse = true;
bool InputManager::s_Wireframe = false;
bool InputManager::s_IsRotating = false; // ADDED

void InputManager::Init(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    s_LastX = (float)width / 2.0f;
    s_LastY = (float)height / 2.0f;

    // Set GLFW callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback); // ADDED
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // FIX: Free the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    // Per-frame input processing like continuous key presses can go here
}

void InputManager::SetCamera(Camera* camera) {
    s_Camera = camera;
}

void InputManager::SetUIManager(UIManager* manager) {
    s_UIManager = manager;
}

// --- GLFW Callbacks ---
void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        s_Wireframe = !s_Wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, s_Wireframe ? GL_LINE : GL_FILL);
    }
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xpos_f = static_cast<float>(xpos);
    float ypos_f = static_cast<float>(ypos);

    if (s_FirstMouse) {
        s_LastX = xpos_f;
        s_LastY = ypos_f;
        s_FirstMouse = false;
    }

    float xoffset = xpos_f - s_LastX;
    float yoffset = s_LastY - ypos_f; 
    s_LastX = xpos_f;
    s_LastY = ypos_f;

    // FIX: Only rotate camera when right mouse button is held down
    if (s_IsRotating && s_Camera) {
        s_Camera->ProcessMouseMovement(xoffset, yoffset);
    }

    // Pass mouse movement to UI manager for hover effects
    if (s_UIManager) {
        s_UIManager->HandleMouseMove(xpos_f, ypos_f);
    }
}

// FIX: New callback to handle mouse clicks
void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // Handle camera rotation
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            s_IsRotating = true;
            s_FirstMouse = true; // Reset to avoid jumping
        } else if (action == GLFW_RELEASE) {
            s_IsRotating = false;
        }
    }

    // Handle UI interaction
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && s_UIManager) {
        s_UIManager->HandleClick(s_LastX, s_LastY);
    }
}

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (s_UIManager) {
        s_UIManager->OnWindowResize(width, height);
    }
}