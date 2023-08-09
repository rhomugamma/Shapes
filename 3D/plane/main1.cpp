#include <GL/glut.h>

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

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 10.0);
}

void keyboard(unsigned char key, int x, int y) {
    const float cameraSpeed = 0.1f;
    switch (key) {
        case 'w': // Move the camera forward
            cameraZ -= cameraSpeed;
            break;
        case 's': // Move the camera backward
            cameraZ += cameraSpeed;
            break;
        case 'a': // Move the camera left
            cameraX -= cameraSpeed;
            break;
        case 'd': // Move the camera right
            cameraX += cameraSpeed;
            break;
    }
    glutPostRedisplay(); // Mark the window for redrawing
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Plane");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); // Register the keyboard event handler

    glutMainLoop();
    return 0;
}
