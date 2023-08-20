#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
/* #include <glm/gtc/matrix_transform.hpp> */
#include <glm/gtc/type_ptr.hpp>
/* #include <assimp/Importer.hpp> */
/* #include <assimp/scene.h> */
/* #include <assimp/postprocess.h> */


const char* vertexShaderSource = R"(

	#version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
    
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    
	}

)";


const char* fragmentShaderSource = R"(

	#version 330 core
    out vec4 FragColor;

    void main() {

		FragColor = vec4(1.0, 0.5, 0.2, 1.0);

	}

)";


float cameraYaw = -90.0f;   // Initialized to face along negative z-axis
float cameraPitch = 0.0f;   // Initialized to zero
float cameraSensitivity = 0.1f;

double lastX = 400.0f;
double lastY = 300.0f;

// Define camera properties
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(0.0f); // Add this line
float cameraSpeed = 0.05f;

bool isMousePressed = false;


class Model {

	public:

		std::string filename;
		std::vector<float> vertexCoordinates;
		std::vector<float> vertexNormals;
		std::vector<float> textureCoordinates;
		std::vector<int> faces;
		GLuint VAO;

		void initModel() {

			// Load the Leviathan.obj model from file
    		std::ifstream objFile(filename);
    		
			if (!objFile.is_open()) {
        	
				std::cerr << "Failed to open obj file" << std::endl;
    		
			}

			std::string line;
    		
			while (std::getline(objFile, line)) {
	
				std::istringstream iss(line);

				if (!objFile.is_open()) {
        
					std::cerr << "Failed to open obj file" << std::endl;
        			return; // Make sure to exit the function if the file is not opened
    			
				}
        		
				std::string type;

				std::vector<std::string> data;

				int firstS = 0;

				/* std::cout << line << '\n'; */


				for (int i = 0; i < line.size(); i++) {

					if (line[i] == ' ') {

					    if (i >= 0 && i < line.length()) {

							std::string type = line.substr(firstS, i - firstS);

							data.push_back(type);
							
							firstS = i + 1;

							/* std::cout << type << '\n'; */

						} 

													
						else {

							std::cout << "Invalid position specified." << '\n';
    						
						}			

					}

					if (line[i] == '/') {

						if (i >= 0 && i < line.length()) {

							std::string type = line.substr(firstS, i - firstS);

							data.push_back(type);
							
							firstS = i + 1;

							/* std::cout << type << '\n'; */

						} 

													
						else {

							std::cout << "Invalid position specified." << '\n';
    						
						}			

					}

					if (i == line.size() - 1) {

						if (i >= 0 && i < line.length()) {

							std::string type = line.substr(firstS, line.length() - firstS);

							data.push_back(type);
							
							firstS = i + 1;

							/* std::cout << type << '\n'; */

						} 

													
						else {

							std::cout << "Invalid position specified." << '\n';
    						
						}

					}
					
				}

        		
				iss >> type;

				if (type == "v") {
            	
					vertexCoordinates.push_back(std::stof(data[1]));
					vertexCoordinates.push_back(std::stof(data[2]));
					vertexCoordinates.push_back(std::stof(data[3]));

       			} 

				if (type == "vn") {

					vertexNormals.push_back(std::stof(data[1]));
					vertexNormals.push_back(std::stof(data[2]));
					vertexNormals.push_back(std::stof(data[3]));

				}

				if (type == "vt") {

					textureCoordinates.push_back(std::stof(data[1]));
					textureCoordinates.push_back(std::stof(data[2]));
					textureCoordinates.push_back(std::stof(data[3]));

				}
			
				if (type == "f") {

					faces.push_back(std::stoi(data[1]) - 1); // OBJ indices are 1-based
					faces.push_back(std::stoi(data[2]) - 1);
					faces.push_back(std::stoi(data[3]));
					faces.push_back(std::stoi(data[4]));
					faces.push_back(std::stoi(data[5]));
					faces.push_back(std::stoi(data[6]));
					faces.push_back(std::stoi(data[7]));
					faces.push_back(std::stoi(data[8]));
					faces.push_back(std::stoi(data[9]));

				}
	
			}

		}
	

		void renderModel() {

		    glGenVertexArrays(1, &VAO);
    	    glBindVertexArray(VAO);

	        GLuint vertexCoordinatesVBO;
        	glGenBuffers(1, &vertexCoordinatesVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoordinates.size(), vertexCoordinates.data(), GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	        glEnableVertexAttribArray(0);
			
			GLuint vertexNormalsVBO;
        	glGenBuffers(1, &vertexNormalsVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNormals.size(), vertexNormals.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

			GLuint textureCoordinatesVBO;
        	glGenBuffers(1, &textureCoordinatesVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoordinates.size(), textureCoordinates.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

			GLuint facesVBO;
        	glGenBuffers(1, &facesVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, facesVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * faces.size(), faces.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);


		}


		void displayModel() {

			glBindVertexArray(VAO);
        	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
        	glBindVertexArray(0);

		}


		void cleanUpModel() {

			glDeleteVertexArrays(1, &VAO);

		}

};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);

}

void processInput(GLFWwindow* window) {

	// Calculate delta time to smooth out camera movement
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // Mouse button event handling
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!isMousePressed) {
            isMousePressed = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }

        // Update yaw and pitch based on mouse input
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top

        lastX = xpos;
        lastY = ypos;

        xOffset *= cameraSensitivity;
        yOffset *= cameraSensitivity;

        cameraYaw += xOffset;
        cameraPitch += yOffset;

        // Clamp pitch to avoid flipping the camera
        if (cameraPitch > 89.0f)
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;

        // Update camera front, right, and up vectors
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(front);
        cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    } 
	
	else {
    
		isMousePressed = false;
    
	}

}

void drawAxis() {

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


void init(Model& model) {

	model.initModel();
	
}


void display(Model& model, GLuint& shaderProgram, GLFWwindow*& window) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

	model.renderModel();
	model.displayModel();
	model.cleanUpModel();

	drawAxis();

	glm::mat4 modelView = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelView");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glfwSwapBuffers(window);

}


int main() {

	Model model;

	model.filename = "Leviathan.obj";

	init(model);

    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Leviathan", nullptr, nullptr);
   
   	if (!window) {
      
	  	std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    
	}

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {

        std::cerr << "GLEW initialization failed" << std::endl;
        return -1;
    
	}

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for compilation errors and handle them

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for compilation errors and handle them

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors and handle them

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	
	while (!glfwWindowShouldClose(window)) {

		processInput(window); // Handle camera movement
		display(model, shaderProgram, window);
        glfwPollEvents();

    }

    return 0;

}
