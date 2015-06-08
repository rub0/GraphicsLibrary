#include "stdafx.h"
#include "OpenGL.h"
#include <Math/Vector3.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Graphics\shader.h"
#include <soil\SOIL.h>
#include "../Dependencies/glm/glm/glm.hpp"
#include "../Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "../Dependencies/glm/glm/gtc/type_ptr.hpp"
#include "Math\MatrixOperations.h"
#include "Math\TransformUtils.h"
#include "Math\Vector4.h"
#include "Math\Matrix4.h"
#include <type_traits>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
} 

int main()
{

	

	// Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader ourShader("resources/shaders/default.vs", "resources/shaders/default.frag");


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
	GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

	//vertex buffer object, vertex array objetct, element buffer object
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	//bind vertex array
    glBindVertexArray(VAO);

	//bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); 

    glBindVertexArray(0); // Unbind VAO


	//LOAD TEXTURE

	int width1, height1, width2, height2;
	unsigned char* image = SOIL_load_image("resources/textures/siegetank.png", &width1, &height1, 0, SOIL_LOAD_RGB);
	unsigned char* image2 = SOIL_load_image("resources/textures/wall.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);
	GLuint texture1, texture2;

	//setting texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);


	//setting texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	//free the image memory and unbind the texture object
	SOIL_free_image_data(image2);
	

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.getProgram(), "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.getProgram(), "ourTexture2"), 1);

		/*// Create transformations
        // Get matrix's uniform location and set matrix
        GLint modelLoc = glGetUniformLocation(ourShader.getProgram(), "model");
        GLint viewLoc = glGetUniformLocation(ourShader.getProgram(), "view");
        GLint projLoc = glGetUniformLocation(ourShader.getProgram(), "projection");


		Engine::Matrix4 model, view, projection;

		
		model = Math::rotateAround(model, Engine::Vector3(0.5f, 1.0f, 0.0f), (GLfloat)glfwGetTime() * 1.0f);
		view = Math::createTransMatrix(view, Engine::Vector3(0.0f, 0.0f, -3.0f));
		projection = Math::perspectiveMatrix(-45.0f, 4/3, 0.1f, 100.f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.m);*/

		// Create transformations
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
		Engine::Matrix4 view2, projection2;
		view2 = Math::createTransMatrix(view2, Engine::Vector3(0.0f, 0.0f, -3.0f));
		projection2 = Math::perspectiveMatrix(-45.0f, 4/3, 0.1f, 100.f);



        model = glm::rotate(model, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.getProgram(), "model");
        GLint viewLoc = glGetUniformLocation(ourShader.getProgram(), "view");
        GLint projLoc = glGetUniformLocation(ourShader.getProgram(), "projection");
        // Pass them to the shaders
		
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view2.m);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection2.m);

        // Activate shader
        ourShader.Use();  

        // Draw the triangle
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Engine::Vector3 cubePositions[] = {
			Engine::Vector3( 0.0f,  0.0f,  0.0f), 
			Engine::Vector3( 2.0f,  5.0f, -15.0f), 
			Engine::Vector3(-1.5f, -2.2f, -2.5f),  
			Engine::Vector3(-3.8f, -2.0f, -12.3f),  
			Engine::Vector3( 2.4f, -0.4f, -3.5f),  
			Engine::Vector3(-1.7f,  3.0f, -7.5f),  
			Engine::Vector3( 1.3f, -2.0f, -2.5f),  
			Engine::Vector3( 1.5f,  2.0f, -2.5f), 
			Engine::Vector3( 1.5f,  0.2f, -1.5f), 
			Engine::Vector3(-1.3f,  1.0f, -1.5f)  
		};

		glm::vec3 cubePositions2[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

		for(GLuint i = 0; i < 10; i++)
		{
			Engine::Matrix4 model2;
			GLfloat angle = 20.0f*i; 
			model2 = Math::createTransMatrix(model2, cubePositions[i]);
			model2 = Math::rotateAround(model2, Engine::Vector3(1.0f, 0.3f, 0.5f), angle);
			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model2.m);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}


