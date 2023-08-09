#include <fstream>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

#include <GL/freeglut.h>
#include <GL/glext.h>

#define WIDTH 600
#define HEIGHT 600

class Model {
  private:
    class Face {
      public:
        int edge;
        int *vertices;
        int *texcoords;
        int normal;

        Face(int edge, int *vertices, int *texcoords, int normal = -1) {
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };
    std::vector<float *> vertices;
    std::vector<float *> texcoords;
    std::vector<float *> normals;
    std::vector<Face> faces;
    GLuint list;
  public:
    void load(const char *filename) {
        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);
        if (!in.is_open()) {
            printf("Cannot load model %s\n", filename);
            return;
        }
        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();
        float a, b, c;
        for (std::string &line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    vertices.push_back(new float[3]{a, b, c});
                } else if (line[1] == 't') {
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back(new float[2]{a, b});
                } else {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back(new float[3]{a, b, c});
                }
            } else if (line[0] == 'f') {
                int v0, v1, v2, t0, t1, t2, n;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
                int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
                faces.push_back(Face(3, v, NULL, n - 1));
            }
        }
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (Face &face : faces) {
            if (face.normal != -1)
                glNormal3fv(normals[face.normal]);
            else
                glDisable(GL_LIGHTING);
            glBegin(GL_POLYGON);
            for (int i = 0; i < face.edge; i++)
                glVertex3fv(vertices[face.vertices[i]]);
            glEnd();
            if (face.normal == -1)
                glEnable(GL_LIGHTING);
        }
        glEndList();
        printf("Model: %s\n", filename);
        printf("Vertices: %ld\n", vertices.size());
        printf("Texcoords: %ld\n", texcoords.size());
        printf("Normals: %ld\n", normals.size());
        printf("Faces: %ld\n", faces.size());
        for (float *f : vertices)
            delete f;
        vertices.clear();
        for (float *f : texcoords)
            delete f;
        texcoords.clear();
        for (float *f : normals)
            delete f;
        normals.clear();
        faces.clear();
    }
    void draw() { glCallList(list); }
};

Model model;

struct Camera {
    float posX = 0.0f;
    float posY = -30.0f;
    float posZ = -500.0f;
    float yaw = 0.0f;   // Rotation around Y-axis
    float pitch = 30.0f; // Rotation around X-axis
    float speed = 10.0f; // Movement speed
    float sensitivity = 0.2f; // Mouse sensitivity for rotation
};

Camera camera;

/* float cameraDistance = -500.0f;  // Initial camera distance */
/* float cameraRotationX = 30.0f;   // Initial camera rotation angle around X-axis */
/* float cameraRotationY = 45.0f;   // Initial camera rotation angle around Y-axis */
/* float cameraSpeed = 10.0f;       // Camera movement speed */

void updateCamera() {

    glLoadIdentity();
    glTranslatef(-camera.posX, -camera.posY, -camera.posZ);
    glRotatef(camera.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(camera.yaw, 0.0f, 1.0f, 0.0f);

    }

void init() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {-1.0f, 10.0f, 100.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, 1.0, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    model.load("Leviathan.obj");
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();  // Update the camera position and orientation
    model.draw();
    glutSwapBuffers();

	    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
    /* glLoadIdentity(); */
	/* glTranslatef(0.0f, -30.0f, -500.0f); */
    /* glRotatef(30.0f, 1.0f, -1.0f, 0.0f); */
    /* model.draw(); */
    /* glutSwapBuffers(); */

}

void keyboardFunc(unsigned char key, int x, int y) {
    float moveSpeed = camera.speed * 0.1f;
    switch (key) {
        case 'w':
            camera.posX += sin(camera.yaw * M_PI / 180.0f) * moveSpeed;
            camera.posZ += -cos(camera.yaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 's':
            camera.posX -= sin(camera.yaw * M_PI / 180.0f) * moveSpeed;
            camera.posZ -= -cos(camera.yaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 'a':
            camera.posX -= sin((camera.yaw - 90.0f) * M_PI / 180.0f) * moveSpeed;
            camera.posZ -= -cos((camera.yaw - 90.0f) * M_PI / 180.0f) * moveSpeed;
            break;
        case 'd':
            camera.posX += sin((camera.yaw - 90.0f) * M_PI / 180.0f) * moveSpeed;
            camera.posZ += -cos((camera.yaw - 90.0f) * M_PI / 180.0f) * moveSpeed;
            break;
    }
    glutPostRedisplay();
}

void specialFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camera.posY += camera.speed * 0.1f;
            break;
        case GLUT_KEY_DOWN:
            camera.posY -= camera.speed * 0.1f;
            break;
        case GLUT_KEY_LEFT:
            camera.yaw -= 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            camera.yaw += 2.0f;
            break;
    }
    glutPostRedisplay();
}

void mouseFunc(int x, int y) {
    static int lastX = x;
    static int lastY = y;

    float xOffset = (x - lastX) * camera.sensitivity;
    float yOffset = (lastY - y) * camera.sensitivity;

    camera.yaw += xOffset;
    camera.pitch += yOffset;

    // Limit pitch to prevent flipping
    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    } else if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Load Model");
    init();
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
	glutPassiveMotionFunc(mouseFunc);
	glutMainLoop();
    return 0;
}
