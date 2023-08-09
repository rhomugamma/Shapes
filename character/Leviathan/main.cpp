#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL/SOIL.h>

// Function to load texture from file
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

    if (!image) {
        std::cout << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

int main() {
    // Initialize GLFW and create a window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Leviathan Model", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Load Leviathan.dae model using Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("Leviathan.dae", aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error loading model: " << importer.GetErrorString() << std::endl;
        return -1;
    }

    // Load texture based on existence of Leviathan_E.png and Leviathan_N.png
    GLuint textureID;
    if (SOIL_check_for_OGL_error() == 0) {
        textureID = loadTexture("Leviathan_E.png");
    }
    else if (SOIL_check_for_OGL_error() == 0) {
        textureID = loadTexture("Leviathan_N.png");
    }
    else {
        textureID = loadTexture("Leviathan.png");
    }

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the model with the loaded texture

        // ...

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glfwTerminate();
    return 0;
}

