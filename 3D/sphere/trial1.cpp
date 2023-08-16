#include <GL/glut.h>
#include <cmath>

// Function to draw the red sphere
void drawRedSphere() {
    glColor3f(1.0f, 0.0f, 0.0f); // Set color to red

    int slices = 50; // Number of subdivisions around the sphere
    int stacks = 50; // Number of subdivisions along the sphere

    glutSolidSphere(1.0, slices, stacks); // Draw a solid sphere with radius 1.0
}

// Function to display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Look-at position
              0.0, 1.0, 0.0); // Up direction

    drawRedSphere(); // Draw the red sphere

    glutSwapBuffers(); // Swap the buffers
}

// Function called when the window is resized
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Red Sphere Example");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
