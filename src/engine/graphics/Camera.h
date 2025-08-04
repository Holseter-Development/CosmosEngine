// src/engine/graphics/Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front; // FIX: This was missing
    glm::vec3 Target;

    // Orbit Controls
    float Radius;
    float Yaw;
    float Pitch;
    
    // Camera options
    float MouseSensitivity;

    // Constructor for an orbiting camera
    Camera(glm::vec3 target = glm::vec3(0.0f), float radius = 5.0f);

    // Returns the view matrix
    glm::mat4 GetViewMatrix();

    // Processes mouse movement to update the orbit angles
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    // Recalculates the camera's position based on its current angles and radius
    void updateCameraPosition();
};
#endif