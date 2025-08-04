// src/engine/graphics/Camera.cpp
#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 target, float radius) : MouseSensitivity(0.1f) {
    Target = target;
    Radius = radius;
    Yaw = -90.0f;
    Pitch = 20.0f;
    updateCameraPosition(); // Initial calculation of Position and Front
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    Yaw += xoffset * MouseSensitivity;
    Pitch -= yoffset * MouseSensitivity;

    // Constrain pitch to avoid flipping
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    updateCameraPosition();
}

void Camera::updateCameraPosition() {
    // Calculate the new Position based on Yaw, Pitch, and Radius from the Target
    Position.x = Target.x + Radius * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Position.y = Target.y + Radius * sin(glm::radians(Pitch));
    Position.z = Target.z + Radius * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    // FIX: Update the Front vector so it always points from the camera to the target
    Front = glm::normalize(Target - Position);
}