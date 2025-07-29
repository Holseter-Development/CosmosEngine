#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 target, float radius) : MouseSensitivity(0.1f) {
    Target = target;
    Radius = radius;
    Yaw = -90.0f;
    Pitch = 20.0f; // Start with a bit of an angle
    updateCameraPosition();
}

glm::mat4 Camera::GetViewMatrix() {
    // Manually construct the view matrix to avoid gimbal lock issues with glm::lookAt
    // This provides smooth, continuous orbiting.
    glm::mat4 view = glm::mat4(1.0f);
    // 4. Pull the world back from the camera (or push the camera out)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -Radius)); 
    // 3. Rotate the world based on the camera's pitch (X-axis rotation)
    view = glm::rotate(view, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f)); 
    // 2. Rotate the world based on the camera's yaw (Y-axis rotation)
    view = glm::rotate(view, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f)); 
    // 1. Move the world so the target is at the origin
    view = glm::translate(view, -Target); 
    return view;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    Yaw   += xoffset * MouseSensitivity;
    // Invert the Y-axis by subtracting the offset
    Pitch -= yoffset * MouseSensitivity;

    // The pitch constraint has been removed to allow for continuous vertical rotation.

    // Update the camera's world position for lighting calculations
    updateCameraPosition();
}

void Camera::updateCameraPosition() {
    // This calculation is now only for sending the camera's position to shaders (e.g., for lighting)
    // It is no longer used to create the view matrix.
    Position.x = Target.x + Radius * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Position.y = Target.y + Radius * sin(glm::radians(Pitch));
    Position.z = Target.z + Radius * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
}
