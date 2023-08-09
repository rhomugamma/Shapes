#include <GL/glew.h>
#include <GLFW/glfw3.h>

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 5.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,  // Eye position
              0.0, 0.0, 0.0,            // Center point
              0.0, 1.0, 0.0);           // Up vector

    // Draw the plane
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.5, 0.5); // Grey color
    glVertex3f(-1.0, -1.0, 0.0); // Bottom-left vertex
    glVertex3f(1.0, -1.0, 0.0);  // Bottom-right vertex
    glVertex3f(1.0, 1.0, 0.0);   // Top-right vertex
    glVertex3f(-1.0, 1.0, 0.0);  // Top-left vertex
    glEnd();

    glfwSwapBuffers(glfwGetCurrentContext()); // Swap the front and back buffers
}

void reshape(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 10.0);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.1f;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_W: // Move the camera forward
                cameraZ -= cameraSpeed;
                break;
            case GLFW_KEY_S: // Move the camera backward
                cameraZ += cameraSpeed;
                break;
            case GLFW_KEY_A: // Move the camera left
                cameraX -= cameraSpeed;
                break;
            case GLFW_KEY_D: // Move the camera right
                cameraX += cameraSpeed;
                break;
        }
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Plane", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    // Set up OpenGL states
    glEnable(GL_DEPTH_TEST);

    // Register callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, keyboard);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        display();
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
