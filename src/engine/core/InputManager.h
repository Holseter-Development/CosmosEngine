#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>
#include "../graphics/Camera.h"

// A static class to handle all input
class InputManager {
public:
    // Initializes the InputManager and sets GLFW callbacks
    static void Init(GLFWwindow* window);
    // Processes input that should be checked every frame (like key presses)
    static void ProcessInput(GLFWwindow* window);
    // Connects the InputManager to the camera it should control
    static void SetCamera(Camera* camera);

private:
    // GLFW callback functions
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Pointer to the scene's camera
    static Camera* s_Camera;
    // Mouse position state
    static float s_LastX;
    static float s_LastY;
    static bool s_FirstMouse;
};

#endif