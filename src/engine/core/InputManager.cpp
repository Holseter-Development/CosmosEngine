#include "InputManager.h"
#include <glad/glad.h>
#include "../ui/UIManager.h"

// Initialize static variables
Camera* InputManager::s_Camera = nullptr;
float InputManager::s_LastX = 0.0f;
float InputManager::s_LastY = 0.0f;
bool InputManager::s_FirstMouse = true;
bool InputManager::s_Wireframe = false;
UIManager* InputManager::s_UIManager = nullptr;

void InputManager::Init(GLFWwindow* window) {
    // Set the static member variables for screen dimensions
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    s_LastX = (float)width / 2.0f;
    s_LastY = (float)height / 2.0f;

    // Set GLFW callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Tell GLFW to capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::ProcessInput(GLFWwindow* window) {
    // Close the application if the ESC key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && s_UIManager) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float uiY = static_cast<float>(height) - s_LastY;
        s_UIManager->HandleClick(s_LastX, uiY);
    }
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
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS && s_UIManager) {
        s_UIManager->TogglePower();
    }
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (s_FirstMouse) {
        s_LastX = xpos;
        s_LastY = ypos;
        s_FirstMouse = false;
    }

    float xoffset = xpos - s_LastX;
    float yoffset = s_LastY - ypos; // reversed since y-coordinates go from bottom to top

    s_LastX = xpos;
    s_LastY = ypos;

    if (s_Camera) {
        s_Camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}