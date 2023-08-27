#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 FragPos;
    void main()
    {
        FragPos = aPos;
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 FragPos;
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(normalize(FragPos), 1.0);
    }
)";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sphere", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

	// Define sphere vertices
const int sectors = 36;
const int stacks = 18;
const float radius = 0.25;
GLfloat vertexData[(stacks + 1) * (sectors + 1) * 3];
GLuint indexData[stacks][sectors * 2 + 2];

int count = 0;
for (int i = 0; i <= stacks; ++i)
{
    double stackAngle = M_PI / 2 - i * M_PI / stacks;
    double xy = radius * cos(stackAngle);
    double z = radius * sin(stackAngle);

    for (int j = 0; j <= sectors; ++j)
    {
        double sectorAngle = j * 2 * M_PI / sectors;
        GLfloat x = xy * cos(sectorAngle);
        GLfloat y = xy * sin(sectorAngle);

        vertexData[count++] = x;
        vertexData[count++] = y;
        vertexData[count++] = z;
    }
}

for (int i = 0; i < stacks; ++i)
{
    for (int j = 0; j <= sectors; ++j)
    {
        int first = i * (sectors + 1) + j;
        int second = first + sectors + 1;

        indexData[i][j * 2] = second;
        indexData[i][j * 2 + 1] = first;
    }

    // Close the triangle strip loop for each stack
    indexData[i][sectors * 2] = indexData[i][sectors * 2 + 1] = indexData[i][0];
}


    // Create VAO, VBO, and EBO
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set up matrices for transformation
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

// Get uniform locations
GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

// Rendering loop
while (!glfwWindowShouldClose(window))
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set uniform matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Update model matrix to rotate and translate sphere
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // Translate
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f)); // Rotate

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draw the sphere
    glBindVertexArray(vao);
    for (int i = 0; i < stacks; ++i)
    {
        glDrawElements(GL_TRIANGLE_STRIP, sectors * 2, GL_UNSIGNED_INT, (void*)(i * sectors * 2 * sizeof(GLuint)));
    }
    glBindVertexArray(0);

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
}



    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glfwTerminate();
    return 0;
}
