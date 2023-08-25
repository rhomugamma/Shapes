#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


const char* vertexShaderSource = R"(

	#version 450 core
    layout (location = 0) in vec3 aPos;
	uniform mat4 view;
	uniform mat4 modelView;
    uniform mat4 projection;

    void main() {
    
       	gl_Position = view * projection * modelView * vec4(aPos, 1.0);

	}

)";


const char* fragmentShaderSource = R"(

	#version 450 core
    out vec4 FragColor;

    void main() {

		FragColor = vec4(1.0, 1.0, 1.0, 1.0);

	}

)";


float cameraYaw = -0.0f;   // Initialized to face along negative z-axis
float cameraPitch = 0.0f;   // Initialized to zero
float cameraSensitivity = 0.1f;

double lastX = 400.0f;
double lastY = 300.0f;

// Define camera properties
glm::vec3 cameraPos = glm::vec3(20.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(-20.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(0.0f); // Add this line
float cameraSpeed = 0.05f;

bool isMousePressed = false;


class Model {

	public:

		std::string filename;
		std::vector<glm::vec3> vertexCoordinates;
		std::vector<glm::vec3> normalCoordinates;
		std::vector<glm::vec3> textureCoordinates;
		std::vector<GLuint> vertexIndices;
		std::vector<GLuint> normalIndices;
		std::vector<GLuint> textureIndices;
		GLuint vertexVBO, vertexVAO, vertexEBO, normalVBO, normalVAO, normalEBO, textureVBO, textureVAO, textureEBO;

		void initModel() {

			// Load the Leviathan.obj model from file
    		std::ifstream objFile(filename);
    		
			if (!objFile.is_open()) {
        	
				std::cerr << "Failed to open obj file" << "\n";
    		
			}

			std::string line;
    		
			while (std::getline(objFile, line)) {
	
				std::istringstream iss(line);

				if (!objFile.is_open()) {
        
					std::cerr << "Failed to open obj file" << "\n";
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

					glm::vec3 vertex;
            		
					vertex.x = std::stof(data[1]);
					vertex.y = std::stof(data[2]);
					vertex.z = std::stof(data[3]);

					vertexCoordinates.push_back(vertex);

       			} 

				if (type == "vn") {

					glm::vec3 normal;
            		
					normal.x = std::stof(data[1]);
					normal.y = std::stof(data[2]);
					normal.z = std::stof(data[3]);

					normalCoordinates.push_back(normal);

				}

				if (type == "vt") {

					glm::vec3 texture;
            		
					texture.x = std::stof(data[1]);
					texture.y = std::stof(data[2]);
					texture.z = std::stof(data[3]);

					textureCoordinates.push_back(texture);

				}
			
				if (type == "f") {

					for (int i = 1; i < data.size(); i += 3) {

						/* std::cout << data[i] << '\n'; */
						/* std::cout << data[i + 1] << '\n'; */
						/* std::cout << data[i + 2] << '\n'; */

						vertexIndices.push_back(std::stoul(data[i]) - 1);
						normalIndices.push_back(std::stoul(data[i + 1]) - 1);
						textureIndices.push_back(std::stoul(data[i + 2]) - 1);

						/* std::cout << "--" << '\n'; */

					}

				}
	
			}

		}
	

		void renderModel() {

		    glGenVertexArrays(1, &vertexVAO);
			glGenBuffers(1, &vertexVBO);
			glGenBuffers(1, &vertexEBO);
    	    /* glBindVertexArray(VAO); */

			glBindVertexArray(vertexVAO);
			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
			glBufferData(GL_ARRAY_BUFFER, vertexCoordinates.size() * sizeof(glm::vec3), vertexCoordinates.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), vertexIndices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);


			glGenVertexArrays(1, &normalVAO);
			glGenBuffers(1, &normalVBO);
			glGenBuffers(1, &normalEBO);
    	    /* glBindVertexArray(VAO); */

			glBindVertexArray(normalVAO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferData(GL_ARRAY_BUFFER, normalCoordinates.size() * sizeof(glm::vec3), normalCoordinates.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, normalIndices.size() * sizeof(GLuint), normalIndices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(1);


			glGenVertexArrays(1, &textureVAO);
			glGenBuffers(1, &textureVBO);
			glGenBuffers(1, &textureEBO);
    	    /* glBindVertexArray(VAO); */

			glBindVertexArray(textureVAO);
			glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
			glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec3), textureCoordinates.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, textureIndices.size() * sizeof(GLuint), textureIndices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(2);
			
        	glBindVertexArray(0);

		}


		void displayModel() {

			renderModel();

			glBindVertexArray(vertexVAO);
        	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
        	glBindVertexArray(0);

			glBindVertexArray(normalVAO);
        	glDrawElements(GL_TRIANGLES, normalIndices.size(), GL_UNSIGNED_INT, 0);
        	glBindVertexArray(0);

			glBindVertexArray(textureVAO);
        	glDrawElements(GL_TRIANGLES, textureIndices.size(), GL_UNSIGNED_INT, 0);
        	glBindVertexArray(0);

		}


		void cleanUpModel() {

			glDeleteBuffers(1, &vertexVBO);
			glDeleteVertexArrays(1, &vertexVAO);
			glDeleteBuffers(1, &vertexEBO);

			glDeleteBuffers(1, &normalVBO);
			glDeleteVertexArrays(1, &normalVAO);
			glDeleteBuffers(1, &normalEBO);

			glDeleteBuffers(1, &textureVBO);
			glDeleteVertexArrays(1, &textureVAO);
			glDeleteBuffers(1, &textureEBO);

			for (int i = 0; i < vertexCoordinates.size(); ++i) {
        
				const glm::vec3& vertex = vertexCoordinates[i];
        		std::cout << "Element " << i << ": X: " << vertex.x << ", Y: " << vertex.y << ", Z: " << vertex.z << std::endl;
    	
			}

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

    cameraSpeed = 2.5f * deltaTime;

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


void displayAxis() {

	glBegin(GL_LINES);

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

}


void init(Model& model) {
	
	model.initModel();
	
}


void display(Model& model, GLuint& shaderProgram, GLFWwindow*& window) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

	glm::mat4 modelView = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelView");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	model.displayModel();

	displayAxis();
	
	glfwSwapBuffers(window);
	
}


int main() {

	Model model;

	model.filename = "Leviathan.obj";

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

	init(model);
	
	while (!glfwWindowShouldClose(window)) {

		processInput(window); // Handle camera movement
		display(model, shaderProgram, window);
        glfwPollEvents();
		
    }

	glDeleteProgram(shaderProgram);

	model.cleanUpModel();

	glfwTerminate();

    return 0;

}
