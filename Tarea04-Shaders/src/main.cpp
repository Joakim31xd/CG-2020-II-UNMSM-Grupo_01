/*
 * main.cpp
 *
 *  Created on: 17 nov. 2020
 *      Author: Hiroshi
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLFWwindow* window;

using namespace glm;

GLint WindowWidth = 600;
GLint WindowHeight = 600;
GLuint programID;
GLfloat* m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;
////////////////////////////////////////////
GLuint m_CBO;
///////////////////////////////////////////
void init (GLFWwindow* window) {

	// Utils
	programID = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// The first 3 points are to Vertex position of Triangle
    n_Vertices = 18;
    m_Vertices = new GLfloat[18] {
			-1.0f, -1.0f, 0.0f, //Triangle 01
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, //Triangle 02
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
	};

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			18 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Política de acesso aos dados, para otimização
		);

	// Nesse ponto, ja copiamos nossos dados pra GPU.
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(GLvoid*) 0
		);

	glEnableVertexAttribArray(0);	// Habilita este atributo

	glBindVertexArray(0);

}


//--------------------------------------------------------------------------------
// void <-----> GLFWwindow* window
void transferDataToGPUMemory13(GLFWwindow* window){	//PERRO

	// VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create and compile our GLSL program from the shaders
    programID = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    /*
	///////////////////////////////////////////////////////////////////////////////////////
	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	///////////////////////////////////////////////////////////////////////////////////////
	*/

    static const GLfloat g_vertex_buffer_data[] = {
        //cuerpo
    	0.0f,  0.0f,  0.0f,
        40.0f, 0.0f,  0.0f,
        40.0f, 20.0f, 0.0f,
        0.0f,  0.0f,  0.0f,
        40.0f, 20.0f, 0.0f,
        0.0f,  20.0f, 0.0f,
		//pata1
        0.0f,  0.0f, 0.0f,
        0.0f, -12.0f, 0.0f,
        5.0f, 0.0f, 0.0f,
		0.0f, -12.0f,  0.0f,
		5.0f, -12.0f,  0.0f,
		5.0f,  0.0f,  0.0f,
		//pata2
		7.0f,  0.0f,  0.0f,
		7.0f,  -12.0f,  0.0f,
		12.0f,  0.0f,  0.0f,
		7.0f,  -12.0f,  0.0f,
		12.0f,  -12.0f,  0.0f,
		12.0f,  0.0f,  0.0f,
		//pata3
		28.0f,  0.0f,  0.0f,
		28.0f, -12.0f,  0.0f,
		33.0f,  0.0f,  0.0f,
		28.0f, -12.0f,  0.0f,
		33.0f,  -12.0f,  0.0f,
		33.0f,  0.0f,  0.0f,
		//pata4
		35.0f,  0.0f,  0.0f,
		35.0f, -12.0f,  0.0f,
		40.0f,  0.0f,  0.0f,
		35.0f, -12.0f,  0.0f,
		40.0f, -12.0f,  0.0f,
		40.0f,  0.0f,  0.0f,
		//cabeza
		0.0f, 20.0f,  0.0f,
		0.0f, 32.0f,  0.0f,
		12.0f, 32.0f,  0.0f,
		0.0f, 20.0f,  0.0f,
		12.0f, 32.0f,  0.0f,
		12.0f, 20.0f,  0.0f,
		//nariz
		-6.0f,  20.0f,  0.0f,
		-6.0f, 26.0f,  0.0f,
		0.0f,  26.0f,  0.0f,
		-6.0f, 20.0f,  0.0f,
		0.0f, 20.0f,  0.0f,
		0.0f,  26.0f,  0.0f,
		//cola
		34.0f,  20.0f,  0.0f,
		40.0f, 20.0f,  0.0f,
		38.0f,  27.0f,  0.0f,
		//oreja1
		0.0f,  32.0f,  0.0f,
		2.0f, 36.0f,  0.0f,
		4.0f,  32.0f,  0.0f,
		//oreja2
		8.0f,  32.0f,  0.0f,
		10.0f, 36.0f,  0.0f,
		12.0f,  32.0f,  0.0f,
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        //cuerpo
        0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		//pata1
        0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		//pata2
        0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		//pata3
        0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		//pata4
        0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		//cabeza
        0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		0.8f,  0.4f,  0.3f,
		//nariz
        0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		0.9f,  0.53f,  0.47f,
		//cola
        0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
		//oreja1
        0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
		//oreja2
        0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
		0.6f,  0.6f,  0.6f,
    };

    // Move vertex data to video memory; specifically to VBO called m_VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data,
        GL_STATIC_DRAW);

    // Move color data to video memory; specifically to CBO called m_CBO
    glGenBuffers(1, &m_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(g_color_buffer_data),
        g_color_buffer_data,
        GL_STATIC_DRAW);

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////;

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
    glVertexAttribPointer(
                          1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                          3,                                // size
                          GL_FLOAT,                         // type
                          GL_FALSE,                         // normalized?
                          0,                                // stride
                          (void*)0                          // array buffer offset
                          );

	//glEnableVertexAttribArray(0);	// Habilita este atributo

	glBindVertexArray(0);

    ///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

}

void draw(double currentTime,GLFWwindow* window){
	glUseProgram(programID);
	GLuint uniformResolution = glGetUniformLocation(programID, "u_resolution");
	//glm::vec2 u_resolution = glm::vec2(WindowWidth,WindowHeight);
	glUniform2f(uniformResolution, 800.0f,800.0f);
	GLuint uniformTime = glGetUniformLocation(programID, "u_time");
	glUniform1f(uniformTime, currentTime);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


int main (void){

	// Initialise GLFW
	 if (!glfwInit()) {
	    	exit(EXIT_FAILURE);
	    }
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	    GLFWwindow* window = glfwCreateWindow(800, 800, "Tarea04 - Shaders", NULL, NULL);
	    glfwMakeContextCurrent(window);
	    if (glewInit() != GLEW_OK) {
	    	exit(EXIT_FAILURE);
	    }
	    glfwSwapInterval(1);

	    //init(window);
	    //double CurrentTime=glfwGetTime();

	    while (!glfwWindowShouldClose(window)) {
	     init(window);
	     //transferDataToGPUMemory13(window);
	     draw(glfwGetTime(),window);
		 glfwSwapBuffers(window);
		 glfwPollEvents();

		}


	glDeleteProgram(programID);

	glfwTerminate();
	return 0;
}
