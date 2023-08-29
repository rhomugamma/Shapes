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
	layout(location = 1) in vec3 color;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 FragPos;
    void main()
    {
        FragPos = color;
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
        /* FragColor = vec4(normalize(FragPos), 1.0); */
		FragColor = vec4(FragPos, 1.0);
    }
)";


class ModelBox {

	public:

		std::vector<float> vertex;
		std::vector<float> color;
		float minX = -5.0;
		float maxX = 5.0;
		float minY = -5.0;
		float maxY = 5.0;
		float minZ = -5.0;
		float maxZ = 5.0;
		float e = 1.0;
		float mass = 10000000;


		GLuint VAO, vertexVBO, colorVAO, colorVBO;

		void init() {

			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(minZ);

			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);
			
			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);
			
			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);
			
			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(minZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(minZ);

			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(minZ);


			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);

			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);
			
			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);
			
			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);
			
			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);

			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);


			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(minZ);

			vertex.push_back(minX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);
			
			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);
			
			vertex.push_back(minX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);
			
			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(minZ);

			vertex.push_back(maxX);
			vertex.push_back(maxY);
			vertex.push_back(maxZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(minZ);

			vertex.push_back(maxX);
			vertex.push_back(minY);
			vertex.push_back(maxZ);

			for (int i = 0; i < vertex.size(); i++) {

				color.push_back(1.0);
				color.push_back(0.6);
				color.push_back(0.1);

			}

			glGenVertexArrays(1, &VAO);
    	    glBindVertexArray(VAO);

        	glGenBuffers(1, &vertexVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
	        glEnableVertexAttribArray(0);

        	glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);

		}


		void display() {

			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 2);
			glDrawArrays(GL_LINES, 2, 2);
			glDrawArrays(GL_LINES, 4, 2);
			glDrawArrays(GL_LINES, 6, 2);
			glDrawArrays(GL_LINES, 8, 2);
			glDrawArrays(GL_LINES, 10, 2);
			glDrawArrays(GL_LINES, 12, 2);
			glDrawArrays(GL_LINES, 14, 2);
			glDrawArrays(GL_LINES, 16, 2);
			glDrawArrays(GL_LINES, 18, 2);
			glDrawArrays(GL_LINES, 20, 2);
			glDrawArrays(GL_LINES, 22, 2);
			glBindVertexArray(0);

		}

};


class SimBox {

	public:

		std::vector<float> vertex;
		std::vector<float> color;
		int increment = 3;
		GLuint vertexVAO, vertexVBO, colorVAO, colorVBO;
		float minX, maxX, minY, maxY, minZ, maxZ;

		void init(ModelBox& modelBox) {

			for (int i = 0; i < modelBox.vertex.size(); i++) {

				vertex.push_back(increment * modelBox.vertex[i]);
				color.push_back(1.0);
				color.push_back(1.0);
				color.push_back(0.0);

			}

			minX = increment * modelBox.minX;
			maxX = increment * modelBox.maxX;
			minY = increment * modelBox.minY;
			maxY = increment * modelBox.maxY;
			minZ = increment * modelBox.minZ;
			maxZ = increment * modelBox.maxZ;

			glGenVertexArrays(1, &vertexVAO);
    	    glBindVertexArray(vertexVAO);

        	glGenBuffers(1, &vertexVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
	        glEnableVertexAttribArray(0);

        	glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);

		}


		void display() {

			glBindVertexArray(vertexVAO);
			glDrawArrays(GL_LINES, 0, 2);
			glDrawArrays(GL_LINES, 2, 2);
			glDrawArrays(GL_LINES, 4, 2);
			glDrawArrays(GL_LINES, 6, 2);
			glDrawArrays(GL_LINES, 8, 2);
			glDrawArrays(GL_LINES, 10, 2);
			glDrawArrays(GL_LINES, 12, 2);
			glDrawArrays(GL_LINES, 14, 2);
			glDrawArrays(GL_LINES, 16, 2);
			glDrawArrays(GL_LINES, 18, 2);
			glDrawArrays(GL_LINES, 20, 2);
			glDrawArrays(GL_LINES, 22, 2);
			glBindVertexArray(0);

		}

};


class Sphere {

	public: 


		GLfloat vertexData[(stacks + 1) * (sectors + 1) * 3];
		std::vector<GLfloat> color;
		GLuint indexData[stacks][sectors * 2 + 2];
    	GLuint vertexVAO, vertexVBO, vertexEBO, colorVAO, colorVBO;  	  // Create VAO, VBO, and EBO
		float coordinatesX, coordinatesY, coordinatesZ;
		float velocityX, velocityY, velocityZ;		
		float accelerationX, accelerationY, accelerationZ;
		float deltaTime = 0.0, frameTime = 0.00;
		float mass;
		float e;


		void init(GLuint& shaderProgram) {

			int count = 0;
		
			for (int i = 0; i <= stacks; i++) {

			    double stackAngle = M_PI / 2 - i * M_PI / stacks;
    			double xy = radius * cos(stackAngle);
    			double z = (radius * sin(stackAngle));

    			for (int j = 0; j <= sectors; ++j) {

			        double sectorAngle = j * 2 * M_PI / sectors;
        			GLfloat x = (xy * cos(sectorAngle));
        			GLfloat y = (xy * sin(sectorAngle));

        			vertexData[count++] = x + coordinatesX;
        			vertexData[count++] = y + coordinatesY;
        			vertexData[count++] = z + coordinatesZ;

					color.push_back(1.0);
					color.push_back(0.0);
					color.push_back(0.0);

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
    		glGenBuffers(1, &vertexEBO);

    		glBindVertexArray(vertexVAO);

    		glGenBuffers(1, &vertexVBO);
    		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    		glEnableVertexAttribArray(0);
    		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

			glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

    		glBindBuffer(GL_ARRAY_BUFFER, 0);
    		glBindVertexArray(0);		

		}


		void borderCollision(ModelBox& modelBox) {

			if (coordinatesX + radius > modelBox.maxX || coordinatesX - radius < modelBox.minX) {

				velocityX =	((velocityX) * ((mass) - (modelBox.mass * modelBox.e))) / (mass + modelBox.mass);

			}
					
			if (coordinatesY + radius > modelBox.maxY || coordinatesY - radius < modelBox.minY) {

				velocityY =	((velocityY) * ((mass) - (modelBox.mass * modelBox.e))) / (mass + modelBox.mass);

			}

			else if (coordinatesZ + radius > modelBox.maxZ || coordinatesZ - radius < modelBox.minZ) {

				velocityZ =	((velocityZ) * ((mass) - (modelBox.mass * modelBox.e))) / (mass + modelBox.mass);

			}

		}



		void update(ModelBox& modelBox) {

			GLfloat totalTime = glfwGetTime();
			deltaTime = totalTime - frameTime;
			frameTime = totalTime;

			velocityX += accelerationX * deltaTime;
			velocityY += accelerationY * deltaTime;
			velocityZ += accelerationZ * deltaTime;

			coordinatesX = (coordinatesX) + (velocityX * deltaTime) + ((0.5) * (accelerationX) * (deltaTime * deltaTime));	
			coordinatesY = (coordinatesY) + (velocityY * deltaTime) + ((0.5) * (accelerationY) * (deltaTime * deltaTime));
			coordinatesZ = (coordinatesZ) + (velocityZ * deltaTime) + ((0.5) * (accelerationZ) * (deltaTime * deltaTime));

			int count = 0;
		
			for (int i = 0; i <= stacks; i++) {

			    double stackAngle = M_PI / 2 - i * M_PI / stacks;
    			double xy = radius * cos(stackAngle);
    			double z = (radius * sin(stackAngle));

    			for (int j = 0; j <= sectors; ++j) {

			        double sectorAngle = j * 2 * M_PI / sectors;
        			GLfloat x = (xy * cos(sectorAngle));
        			GLfloat y = (xy * sin(sectorAngle));

        			vertexData[count++] = x + coordinatesX;
        			vertexData[count++] = y + coordinatesY;
        			vertexData[count++] = z + coordinatesZ;

    			}

			}

			borderCollision(modelBox);

			// Update vertex buffer data
    		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		}

		void display(GLuint& shaderProgram) {
	
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


class Axis {

	public:

		float mass = 1000000000;

		GLfloat verticesAxis[18] = {

			  0.0,  0.0,  0.0,
			 10.0,  0.0,  0.0,

			  0.0,  0.0,  0.0,
			  0.0, 10.0,  0.0,

			  0.0,  0.0,  0.0,
			  0.0,  0.0, 10.0

		};

		GLfloat colorAxis[18] {

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f

		};

		GLuint VAO, vertexVBO, colorVBO;


		void init() {

			glGenVertexArrays(1, &VAO);
    	    glBindVertexArray(VAO);

        	glGenBuffers(1, &vertexVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis), verticesAxis, GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	        glEnableVertexAttribArray(0);

        	glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(colorAxis), colorAxis, GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);

		}


		void display() {

			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 2);
			glDrawArrays(GL_LINES, 2, 2);
			glDrawArrays(GL_LINES, 4, 2);
			glBindVertexArray(0);

		}


		void cleanUp() {

			glDeleteVertexArrays(1, &VAO);

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


void init(std::vector<Sphere>& sphereObjects, Axis& axis, ModelBox& modelBox, SimBox& simBox, GLuint& shaderProgram) {

	axis.init();

	float position = -4.5;

	for (int i = 0; i < numberObjects; i++) {

		sphereObjects.push_back(Sphere());
		
		sphereObjects[i].coordinatesX = 0.0;
		sphereObjects[i].coordinatesY = 0.0;
		sphereObjects[i].coordinatesZ = position;

		sphereObjects[i].velocityX = 0.8;
		sphereObjects[i].velocityY = 1.3;
		sphereObjects[i].velocityZ = 0.6;

		sphereObjects[i].accelerationX = 0.0;
		sphereObjects[i].accelerationY = 0.0;
		sphereObjects[i].accelerationZ = 0.0;

		sphereObjects[i].mass = 0.05;
		sphereObjects[i].e = 1.0;

		position += 0.75;

		sphereObjects[i].init(shaderProgram);
	
	}

	modelBox.init();

	simBox.init(modelBox);

}


void display(std::vector<Sphere>& sphereObjects, Axis& axis, ModelBox& modelBox, SimBox& simBox, GLFWwindow* window, GLuint shaderProgram, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
	
 	// Clear the screen
   	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up matrices for transformation

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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

		sphereObjects[i].display(shaderProgram);

	}

	axis.display();

	modelBox.display();

	simBox.display();

	// Swap buffers and poll events
   	glfwSwapBuffers(window);

}


void cleanUp(std::vector<Sphere>& sphereObjects, Axis& axis, GLuint& shaderProgram, GLuint& fragmentShader, GLuint& vertexShader) {

	for (int i = 0; i < sphereObjects.size(); i++) {

		sphereObjects[i].cleanUp();

	}

	axis.cleanUp();

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

}


int main() {

	std::vector<Sphere> sphereObjects;

	Axis axis;

	ModelBox modelBox;

	SimBox simBox;
	 
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


	init(sphereObjects, axis, modelBox, simBox, shaderProgram);

	// Define camera properties
	glm::vec3 cameraPos = glm::vec3(simBox.maxX, simBox.maxY, simBox.maxZ);
	glm::vec3 cameraFront = glm::vec3(-simBox.maxX, -simBox.maxY, -simBox.maxZ);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f);
	float cameraYaw = -0.0f;   // Initialized to face along negative z-axis
	float cameraPitch = 0.0f;   // Initialized to zero

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
 
		processInput(window, cameraPos, cameraFront, cameraRight, cameraUp, cameraYaw, cameraPitch); // Handle camera movement

		for (int i = 0; i < sphereObjects.size(); i++) {

			sphereObjects[i].update(modelBox);

		}

		display(sphereObjects, axis, modelBox, simBox, window, shaderProgram, cameraPos, cameraFront, cameraUp);
		
    	glfwPollEvents();

	}

	cleanUp(sphereObjects, axis, shaderProgram, fragmentShader, vertexShader);

    glfwTerminate();
    return 0;

}
