#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float angle = 0.0f;

void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < slices; i++) {
        float theta1 = static_cast<float>(i) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(slices);
        float theta2 = static_cast<float>(i + 1) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(slices);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= stacks; j++) {
            float phi = static_cast<float>(j) * static_cast<float>(M_PI) / static_cast<float>(stacks);
            float x1 = radius * sinf(phi) * cosf(theta1);
            float y1 = radius * sinf(phi) * sinf(theta1);
            float z1 = radius * cosf(phi);

            float x2 = radius * sinf(phi) * cosf(theta2);
            float y2 = radius * sinf(phi) * sinf(theta2);
            float z2 = radius * cosf(phi);

            glNormal3f(x1, y1, z1);
            glVertex3f(x1, y1, z1);

            glNormal3f(x2, y2, z2);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    drawSphere(1.0f, 50, 50);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}

void idle() {
    angle += 0.5f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("OpenGL Sphere");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
