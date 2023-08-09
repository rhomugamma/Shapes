#include <fstream>
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

float cameraDistance = -500.0f;
float cameraRotationX = 30.0f;
float cameraRotationY = 45.0f;
float cameraSpeed = 10.0f;
float cameraRotationSpeed = 2.0f; // New rotation speed

// New camera position and orientation variables
GLfloat cameraPosition[] = {0.0f, 0.0f, -500.0f};
GLfloat cameraForward[] = {0.0f, 0.0f, -1.0f};
GLfloat cameraRight[] = {1.0f, 0.0f, 0.0f};
GLfloat cameraUp[] = {0.0f, 1.0f, 0.0f};

void updateCamera() {
    glLoadIdentity();
    gluLookAt(
        cameraPosition[0], cameraPosition[1], cameraPosition[2],
        cameraPosition[0] + cameraForward[0], cameraPosition[1] + cameraForward[1], cameraPosition[2] + cameraForward[2],
        cameraUp[0], cameraUp[1], cameraUp[2]
    );
}

void keyboardFunc(unsigned char key, int x, int y) {
    GLfloat cameraSpeedFactor = 0.1f;

    switch (key) {
        case 'w':
            cameraPosition[0] += cameraForward[0] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[1] += cameraForward[1] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[2] += cameraForward[2] * cameraSpeed * cameraSpeedFactor;
            break;
        case 's':
            cameraPosition[0] -= cameraForward[0] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[1] -= cameraForward[1] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[2] -= cameraForward[2] * cameraSpeed * cameraSpeedFactor;
            break;
        case 'a':
            cameraPosition[0] -= cameraRight[0] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[1] -= cameraRight[1] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[2] -= cameraRight[2] * cameraSpeed * cameraSpeedFactor;
            break;
        case 'd':
            cameraPosition[0] += cameraRight[0] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[1] += cameraRight[1] * cameraSpeed * cameraSpeedFactor;
            cameraPosition[2] += cameraRight[2] * cameraSpeed * cameraSpeedFactor;
            break;
    }
    glutPostRedisplay();
}

void specialFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            cameraDistance += cameraSpeed;
            break;
        case GLUT_KEY_DOWN:
            cameraDistance -= cameraSpeed;
            break;
        case GLUT_KEY_LEFT:
            cameraRotationY -= cameraRotationSpeed;
            break;
        case GLUT_KEY_RIGHT:
            cameraRotationY += cameraRotationSpeed;
            break;
    }
    glutPostRedisplay();
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
    glutMainLoop();
    return 0;

}
