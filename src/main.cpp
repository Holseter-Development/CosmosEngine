#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/graphics/Shader.h"
#include "engine/graphics/Camera.h"
#include "engine/core/InputManager.h"
#include "engine/graphics/TextureLoader.h"
#include "engine/graphics/Model.h"
#include "engine/graphics/Sphere.h" // Include our new Sphere class
#include "engine/ui/UIManager.h"
#include "game/CommandModule.h"

// --- Initial Window Dimensions ---
unsigned int screenWidth = 1600;
unsigned int screenHeight = 900;

int main() {
    // 1. Initialize GLFW & create window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Asteroid Lander", NULL, NULL);
    glfwMakeContextCurrent(window);

    // 2. Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 3. Initialize InputManager and Camera
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f);
    InputManager::Init(window);
    InputManager::SetCamera(&camera);

    CommandModule commandModule;
    UIManager uiManager;
    uiManager.Init(screenWidth, screenHeight, &commandModule);
    InputManager::SetUIManager(&uiManager);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    // 4. Load shaders
    Shader modelShader("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
    Shader sunShader("assets/shaders/sun.vert", "assets/shaders/sun.frag");
    Shader skyboxShader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

    // 5. Load the Lander model
    Model ourModel("assets/models/commandmodule/ApolloLunarModule.glb");

    // 6. Create the Sun object
    Sphere sunSphere(1.0f, 36, 18); // A sphere with radius 1
    unsigned int sunTexture = TextureLoader::loadTexture("assets/textures/sun.jpg");
    sunShader.use();
    sunShader.setInt("texture_diffuse1", 0);

    // 7. Setup for Skybox
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    std::vector<std::string> faces { "assets/cubemaps/space/px.jpg", "assets/cubemaps/space/nx.jpg", "assets/cubemaps/space/py.jpg", "assets/cubemaps/space/ny.jpg", "assets/cubemaps/space/pz.jpg", "assets/cubemaps/space/nz.jpg" };
    unsigned int cubemapTexture = TextureLoader::loadCubemap(faces);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Light properties - moved further away
    glm::vec3 lightPos(0.0f, 5.0f, -20.0f);

    // 8. Game Loop
    while (!glfwWindowShouldClose(window)) {
        InputManager::ProcessInput(window);

        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)currentWidth / (float)currentHeight, 1.0f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;

        // --- Draw the Lander Model ---
        modelShader.use();
        modelShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        modelShader.setVec3("lightPos", lightPos);
        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);

        // --- Draw the Sun Sphere ---
        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(2.0f)); // Made it larger
        sunShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        sunSphere.Draw();

        // --- Draw skybox as last ---
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // --- Render UI ---
        uiManager.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup...
    glfwTerminate();
    return 0;
}
