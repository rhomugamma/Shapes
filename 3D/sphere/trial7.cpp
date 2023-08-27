#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Define sphere vertices
const int sectors = 36;
const int stacks = 18;
const float radius = 0.25;
const int numberObjects = 10;
double lastMouseX = 0.0; // Initial X position of the mouse (center of the window)
double lastMouseY = 0.0; // Initial Y position of the mouse (center of the window)
bool mouseButtonPressed = false;


// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 450 core
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
    #version 450 core
    in vec3 FragPos;
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(normalize(FragPos), 1.0);
    }
)";


class Sphere {

	public: 


		GLfloat vertexData[(stacks + 1) * (sectors + 1) * 3];
		GLuint indexData[stacks][sectors * 2 + 2];
    	GLuint vertexVAO, vertexVBO, vertexEBO;  	  // Create VAO, VBO, and EBO
		float coordinatesX, coordinatesY, coordinatesZ;
		float velocityX, velocityY, velocityZ;		
		float accelerationX, accelerationY, accelerationZ;
		float deltaTime = 0.0, frameTime = 0.00;


		void init(GLuint& shaderProgram) {

			int count = 0;
		
			for (int i = 0; i <= stacks; i++) {

			    double stackAngle = M_PI / 2 - i * M_PI / stacks;
    			double xy = radius * cos(stackAngle);
    			double z = coordinatesZ + (radius * sin(stackAngle));

    			for (int j = 0; j <= sectors; ++j) {

			        double sectorAngle = j * 2 * M_PI / sectors;
        			GLfloat x = coordinatesX + (xy * cos(sectorAngle));
        			GLfloat y = coordinatesY + (xy * sin(sectorAngle));

        			vertexData[count++] = x;
        			vertexData[count++] = y;
        			vertexData[count++] = z;

    			}

			}


			for (int i = 0; i < stacks; ++i) {

			    for (int j = 0; j <= sectors; ++j) {

					int first = i * (sectors + 1) + j;
        			int second = first + sectors + 1;

        			indexData[i][j * 2] = second;
        			indexData[i][j * 2 + 1] = first;
    			
				}

    			// Close the triangle strip loop for each stack
   	 			indexData[i][sectors * 2] = indexData[i][sectors * 2 + 1] = indexData[i][0];
			
			}

    		glGenVertexArrays(1, &vertexVAO);
    		glGenBuffers(1, &vertexVBO);
    		glGenBuffers(1, &vertexEBO);

    		glBindVertexArray(vertexVAO);

    		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

    		glEnableVertexAttribArray(0);
    		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    		glBindBuffer(GL_ARRAY_BUFFER, 0);
    		glBindVertexArray(0);		

		}


		void update() {

			GLfloat totalTime = glfwGetTime();
			deltaTime = totalTime - frameTime;
			frameTime = totalTime;

			coordinatesX = (coordinatesX) + (velocityX * deltaTime) + ((1/2) * (accelerationX) * (deltaTime * deltaTime));	
			coordinatesY = (coordinatesY) + (velocityY * deltaTime) + ((1/2) * (accelerationY) * (deltaTime * deltaTime));
			coordinatesZ = (coordinatesZ) + (velocityZ * deltaTime) + ((1/2) * (accelerationZ) * (deltaTime * deltaTime));

			velocityX += accelerationX * deltaTime;
			velocityY += accelerationY * deltaTime;
			velocityZ += accelerationZ * deltaTime;

		}


		void display(glm::mat4 model, GLuint& shaderProgram) {

			// Update model matrix to rotate and translate sphere
   			model = glm::mat4(1.0f);
   			model = glm::translate(model, glm::vec3(coordinatesX, coordinatesY, coordinatesZ)); // Translate
   			model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f)); // Rotate

			// Draw the sphere
    		glBindVertexArray(vertexVAO);
    		
			for (int i = 0; i < stacks; ++i) {

        		glDrawElements(GL_TRIANGLE_STRIP, sectors * 2, GL_UNSIGNED_INT, (void*)(i * sectors * 2 * sizeof(GLuint)));
    		
			}
    		
			glBindVertexArray(0);

		}


		void cleanUp() {

			// Cleanup
    		glDeleteVertexArrays(1, &vertexVAO);
    		glDeleteBuffers(1, &vertexVBO);
    		glDeleteBuffers(1, &vertexEBO);

		}

};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}


void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraRight, glm::vec3& cameraUp, float& cameraYaw, float& cameraPitch) {

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

    // Handle mouse movement
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Calculate the change in mouse position
    double deltaX = mouseX - lastMouseX;
    double deltaY = lastMouseY - mouseY; // Note the inversion since Y-coordinates increase upwards

    // Update last known mouse position
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Sensitivity factor for mouse movement
    float sensitivity = 0.1f;

	    // Check if the mouse button is currently held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		if (!mouseButtonPressed) {

            // Initialize the last known position to the current position to avoid sudden jumps
            lastMouseX = mouseX;
			lastMouseY = mouseY;

		}

		else {

			mouseButtonPressed = true;

			cameraYaw += deltaY * sensitivity;
    		cameraPitch += deltaX * sensitivity;

	    	// Limit the camera pitch to avoid flipping
	    	if (cameraPitch > 89.0f)
    	    	cameraPitch = 89.0f;
    			if (cameraPitch < -89.0f)
        		cameraPitch = -89.0f;

    		// Calculate the new front vector
    		glm::vec3 front;
    		front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    		front.y = sin(glm::radians(cameraPitch));
    		front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    		cameraFront = glm::normalize(front);

		}
	}

	else {

		mouseButtonPressed = false;

	}

}


void init(std::vector<Sphere>& sphereObjects, GLuint& shaderProgram) {

	for (int i = 0; i < numberObjects; i++) {

		sphereObjects.push_back(Sphere());
		
		sphereObjects[i].coordinatesX = 0.0;
		sphereObjects[i].coordinatesY = 0.0;
		sphereObjects[i].coordinatesZ = 0.0;

		sphereObjects[i].velocityX = 0.1;
		sphereObjects[i].velocityY = 0.0;
		sphereObjects[i].velocityZ = 0.0;

		sphereObjects[i].accelerationX = 0.0;
		sphereObjects[i].accelerationY = 0.0;
		sphereObjects[i].accelerationZ = 0.0;

		sphereObjects[i].init(shaderProgram);
	
	}

}


void display(std::vector<Sphere>& sphereObjects, GLFWwindow* window, GLuint shaderProgram) {
	
 	// Clear the screen
   	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Set up matrices for transformation
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

					// Get uniform locations
			GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
			GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
			GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

			// Set uniform matrices
   			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
   			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	for (int i = 0; i < sphereObjects.size(); i++) {

		sphereObjects[i].display(model, shaderProgram);

	}

	// Swap buffers and poll events
   	glfwSwapBuffers(window);

}


int main() {

	std::vector<Sphere> sphereObjects;
	 
    // Initialize GLFW
	if (!glfwInit()) {

		std::cerr << "GLFW initialization failed" << std::endl;
        return -1;

	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sphere", nullptr, nullptr);

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

	// Define camera properties
	glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f);
	float cameraYaw = -0.0f;   // Initialized to face along negative z-axis
	float cameraPitch = 0.0f;   // Initialized to zero

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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	init(sphereObjects, shaderProgram);

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
 
		 processInput(window, cameraPos, cameraFront, cameraRight, cameraUp, cameraYaw, cameraPitch); // Handle camera movement

		for (int i = 0; i < sphereObjects.size(); i++) {

			sphereObjects[i].update();

		}
		display(sphereObjects, window, shaderProgram);
    	glfwPollEvents();

	}

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glfwTerminate();
    return 0;

}
