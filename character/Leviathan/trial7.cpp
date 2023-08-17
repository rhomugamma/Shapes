#include <fstream>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WIDTH 600
#define HEIGHT 600

int prevMouseX, prevMouseY;
bool isMousePressed = false;

float minX = std::numeric_limits<float>::max();
float minY = std::numeric_limits<float>::max();
float minZ = std::numeric_limits<float>::max();

float maxX = std::numeric_limits<float>::min();
float maxY = std::numeric_limits<float>::min();
float maxZ = std::numeric_limits<float>::min();

float OminX;
float OmaxX;

float OminY;
float OmaxY;

float OminZ;
float OmaxZ;

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

					minX = std::min(minX, a);
    		        minY = std::min(minY, b);
    	    	    minZ = std::min(minZ, c);
           			maxX = std::max(maxX, a);
            		maxY = std::max(maxY, b);
            		maxZ = std::max(maxZ, c);
					OminX = 3.25 * minX;
					OmaxX = 3.25 * maxX;
					OminY =  4.75 * minY;
					OmaxY =  4.75 * maxY;
					OminZ = 2.75 * minZ;
					OmaxZ = 2.75 * maxZ;


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

/* Camera Position X: 0 */
/* Camera Position Y: 0 */
/* Camera Position Z: -26 */
/* Camera Rotation X: 76 */
/* Camera Rotation Y: 0 */
/* Camera Rotation Z: 40 */

/* float cameraPositionX = OmaxX;  // Initial camera distance */
/* float cameraPositionY = OmaxY; */
/* float cameraPositionZ = OmaxZ; */

 
float cameraPositionX = 0;  // Initial camera distance
float cameraPositionY = 0;
float cameraPositionZ = -26;


float cameraRotationX = 76.0f;   // Initial camera rotation angle around X-axis
float cameraRotationY = 0.0f;   // Initial camera rotation angle around Y-axis
float cameraRotationZ = 40.0f;
float cameraSpeed = 1.0f;       // Camera movement speed
float cameraRotationSpeed = 1.0f;

void updateCamera() {

	glLoadIdentity();

	glTranslatef(cameraPositionX, cameraPositionY, cameraPositionZ);
   	
    glRotatef(cameraRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotationY, 0.0f, 1.0f, 0.0f);
	glRotatef(cameraRotationZ, 0.0f, 0.0f, 1.0f);
	 
}

/* void keyboardFunc(unsigned char key, int x, int y) { */
void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
	switch (key) {

        case 'w':
            cameraRotationX -= cameraRotationSpeed;
            break;
        case 's':
            cameraRotationX += cameraRotationSpeed;
            break;
        case 'a':
            cameraRotationY -= cameraRotationSpeed;
            break;
        case 'd':
            cameraRotationY += cameraRotationSpeed;
            break;
        case 'q':
            cameraRotationZ -= cameraRotationSpeed;
            break;
        case 'e':
            cameraRotationZ += cameraRotationSpeed;
            break;
		case 'n':
            cameraPositionX += cameraSpeed;
            break;
        case 'm':
            cameraPositionX -= cameraSpeed;
            break;

    }

    /* glutPostRedisplay(); */
	/* glfwPostRedisplay(); */
	glfwPostEmptyEvent();

}

void specialFunc(int key, int x, int y) {

    switch (key) {

        case GLFW_KEY_UP:
            cameraPositionY -= cameraSpeed;
            break;
        case GLFW_KEY_DOWN:
            cameraPositionY += cameraSpeed;
            break;
        case GLFW_KEY_RIGHT:
            cameraPositionZ -= cameraSpeed;
            break;
        case GLFW_KEY_LEFT:
            cameraPositionZ += cameraSpeed;
            break;

    }

    /* glutPostRedisplay(); */
	 /* glfwPostRedisplay(); */
	 glfwPostEmptyEvent();

}

void drawAxes() {
	 
	glBegin(GL_LINES);

	glDisable(GL_LIGHTING);

    // X-axis (red)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    // Y-axis (green)
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    // Z-axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);

	
    glEnd();

	glEnable(GL_LIGHTING);

}

void drawModelBox() {

	glBegin(GL_LINES);

	glDisable(GL_LIGHTING);

	//Bottom face
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(minX, minY, minZ);
	glVertex3f(minX, maxY, minZ);

	glVertex3f(minX, maxY, minZ);
	glVertex3f(maxX, maxY, minZ);

	glVertex3f(maxX, maxY, minZ);
	glVertex3f(maxX, minY, minZ);

	glVertex3f(maxX, minY, minZ);
	glVertex3f(minX, minY, minZ);


	//Upper face
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(minX, maxY, maxZ);

	glVertex3f(minX, maxY, maxZ);
	glVertex3f(maxX, maxY, maxZ);

	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(maxX, minY, maxZ);

	glVertex3f(maxX, minY, maxZ);
	glVertex3f(minX, minY, maxZ);
	

	//Connections
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(minX, minY, minZ);
	glVertex3f(minX, minY, maxZ);

	glVertex3f(minX, maxY, minZ);
	glVertex3f(minX, maxY, maxZ);

	glVertex3f(maxX, maxY, minZ);
	glVertex3f(maxX, maxY, maxZ);

	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);

	glEnd();

	glEnable(GL_LIGHTING);

}

void drawOuterBox() {

	glBegin(GL_LINES);

	glDisable(GL_LIGHTING);

	
	//Bottom face
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(OminX, OminY, OminZ);
	glVertex3f(OminX, OmaxY, OminZ);

	glVertex3f(OminX, OmaxY, OminZ);
	glVertex3f(OmaxX, OmaxY, OminZ);

	glVertex3f(OmaxX, OmaxY, OminZ);
	glVertex3f(OmaxX, OminY, OminZ);

	glVertex3f(OmaxX, OminY, OminZ);
	glVertex3f(OminX, OminY, OminZ);


	//Upper face
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(OminX, OminY, OmaxZ);
	glVertex3f(OminX, OmaxY, OmaxZ);

	glVertex3f(OminX, OmaxY, OmaxZ);
	glVertex3f(OmaxX, OmaxY, OmaxZ);

	glVertex3f(OmaxX, OmaxY, OmaxZ);
	glVertex3f(OmaxX, OminY, OmaxZ);

	glVertex3f(OmaxX, OminY, OmaxZ);
	glVertex3f(OminX, OminY, OmaxZ);
	

	//Connections
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(OminX, OminY, OminZ);
	glVertex3f(OminX, OminY, OmaxZ);

	glVertex3f(OminX, OmaxY, OminZ);
	glVertex3f(OminX, OmaxY, OmaxZ);

	glVertex3f(OmaxX, OmaxY, OminZ);
	glVertex3f(OmaxX, OmaxY, OmaxZ);

	glVertex3f(OmaxX, OminY, OminZ);
	glVertex3f(OmaxX, OminY, OmaxZ);

	glEnd();

	glEnable(GL_LIGHTING);

}

void init(GLFWwindow* window) {
	
	
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {-1.0f, 10.0f, 100.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(-20.0, 1.0, 1.0, 1000.0);
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
	/* glutInitDisplayMode(GL_DOUBLE | GL_RGB | GL_DEPTH | GL_MULTISAMPLE); */
	glfwMakeContextCurrent(window);
	glEnable(GL_COLOR_MATERIAL);

    model.load("Leviathan.obj");

}



void display(GLFWwindow* window) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();  // Update the camera position and orientation
	drawAxes();
	drawModelBox();
	drawOuterBox();
 	model.draw();
    /* glutSwapBuffers(window); */
	glfwSwapBuffers(window);

	/* std::cout << "Camera Position X: " << cameraPositionX << '\n'; */
	/* std::cout << "Camera Position Y: " << cameraPositionY << '\n'; */
	/* std::cout << "Camera Position Z: " << cameraPositionZ << '\n'; */
	/* std::cout << "Camera Rotation X: " << cameraRotationX << '\n'; */
	/* std::cout << "Camera Rotation Y: " << cameraRotationY << '\n'; */
	/* std::cout << "Camera Rotation Z: " << cameraRotationZ << '\n'; */

}

/* void mouseFunc(int button, int state, int x, int y) { */
void mouseFunc(GLFWwindow* window, int button, int action, int mods) {

    /* if (button == GLFW_MOUSE_BUTTON_LEFT) { */
    
		/* if (action == GLFW_KEY_DOWN) { */
    
			/* isMousePressed = true; */
    /*         prevMouseX = x; */
    /*         prevMouseY = y; */
    
		/* } */ 
		
		/* else if (state == GLFW_KEY_UP) { */
    
			/* isMousePressed = false; */
    
		/* } */
    
	/* } */

	     if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isMousePressed = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            prevMouseX = xpos;
            prevMouseY = ypos;
        } else if (action == GLFW_RELEASE) {
            isMousePressed = false;
        }
    }

}

/* void motionFunc(int x, int y) { */
void motionFunc(GLFWwindow* window, double x, double y) {

    if (isMousePressed) {

        int deltaX = x - prevMouseX;
        int deltaY = y - prevMouseY;
        prevMouseX = x;
        prevMouseY = y;

        cameraRotationY += static_cast<float>(deltaX) * 0.2f;
        cameraRotationX += static_cast<float>(deltaY) * 0.2f;

        /* glutPostRedisplay(); */

    }

}


int main(int argc, char **argv) {

	    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Load Model", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Load the model and other setup
    init(window);

    /* glfwSetKeyCallback(window, keyboardFunc); */
    /* glfwSetMouseButtonCallback(window, mouseFunc); */
    /* glfwSetCursorPosCallback(window, motionFunc); */
	 glfwSetKeyCallback(window, keyboardFunc);
glfwSetMouseButtonCallback(window, mouseFunc);
glfwSetCursorPosCallback(window, motionFunc);

    while (!glfwWindowShouldClose(window)) {
        /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

		display(window);
        /* updateCamera(); */
        /* drawAxes(); */
        /* drawModelBox(); */
        /* drawOuterBox(); */
        /* model.draw(); */

        /* glfwSwapBuffers(window); */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
   
}
