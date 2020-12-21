//============================================================================
// Name        : Tarea07_Transformaciones Grupo 01
// Professor   : Herminio Paucar
// Alumnos     : Néstor Sánchez, Juan Diego Córdova, Rosinaldo Rivera
// Description : Utilizamos transformaciones Scale,Rotate and Traslate
//============================================================================

// Include standard headers

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef MPI
#define M_PI 3.14159265358979323846
#endif

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "Utils.h"

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
GLFWwindow* window;

using namespace glm;
using namespace std;

// Vertex array object (VAO)
GLuint m_VAO[4];

// Vertex buffer object (VBO)
GLuint *m_VBO;

// color buffer object (CBO)
GLuint *m_CBO;

// GLSL program from the shaders
GLuint programID;

GLuint VAO_Puntero=0;
GLuint VBO_Puntero=0;
GLuint CBO_Puntero=0;

GLint numeroVertices;

GLint WindowWidth = 600;
GLint WindowHeight = 600;

const float toRadians = 3.14159265f / 180.0f;
float curAngle = 0.0f;
float AnguloSegundero = 0.0f;
float AnguloMinutero = 0.0f;
float paso = 0.0f;
float rotaPendulo = 0.0f;
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 195.0f;
float triMinOffset = -95.0f;
float triIncrement = 0.05425f;//tabien
float decrementopaso=1.0f;
bool key=true,decre=true;
//---------------------------------------------------------------------------------------------------
void GenerarCirculo(GLfloat Radio, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B){

		GLfloat radius = Radio;
		GLint numeroLados = 100;
		numeroVertices = numeroLados+2;
		GLfloat twicePi = 2.0f * M_PI;

		GLfloat verticesX[numeroVertices];
		GLfloat verticesY[numeroVertices];
		GLfloat verticesZ[numeroVertices];

		verticesX[0] = x;
		verticesY[0] = y;
		verticesZ[0] = z;
		for (int i = 0; i < numeroVertices; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numeroLados));
			verticesY[i] = y + (radius * sin(i * twicePi / numeroLados));
			verticesZ[i] = z;
		}
		GLint dimVertices = (numeroVertices) * 3;
		GLfloat m_Vertices[dimVertices];
		GLfloat m_Colores[dimVertices];


		for (int i = 0; i < numeroVertices; i++) {
			m_Vertices[i * 3] = verticesX[i];
			m_Vertices[i * 3 + 1] = verticesY[i];
			m_Vertices[i * 3 + 2] = verticesZ[i];
			m_Colores[i * 3] = R;
			m_Colores[i * 3 + 1] = G;
			m_Colores[i * 3 + 2] = B;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
		glBufferData(
		    GL_ARRAY_BUFFER,
		    sizeof(m_Vertices),
			m_Vertices,
		    GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
			glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(m_Colores),
			m_Colores,
			GL_STATIC_DRAW);

}

void GenerarSemiCirculo(GLfloat Radio, GLfloat x, GLfloat y, GLfloat z,GLuint A,GLfloat R, GLfloat G , GLfloat B){

		GLfloat radius = Radio;
		GLint numeroLados = 100;
		numeroVertices = numeroLados/2;
		GLfloat twicePi = 2.0f * M_PI;
		GLfloat verticesX[numeroVertices];
		GLfloat verticesY[numeroVertices];
		GLfloat verticesZ[numeroVertices];

		verticesX[0] = (radius * cos(A * twicePi / 360));
		verticesY[0] = (radius * sin(A * twicePi/ 360));
		verticesZ[0] = z;
		int aux=numeroLados*A/360;

		for (int i=aux-1; i <aux+numeroVertices; i++) {
			verticesX[i-(aux)] = x + (radius * cos(i * twicePi / numeroLados));
			verticesY[i-(aux)] = y + (radius * sin(i * twicePi / numeroLados));
			verticesZ[i-(aux)] = z;
		}

		GLint dimVertices = ((numeroVertices)) * 3;
		GLfloat m_Vertices[dimVertices];
		GLfloat m_Colores[dimVertices];

		for (int i = 0; i < numeroVertices; i++) {
			m_Vertices[i * 3] = verticesX[i];
			m_Vertices[i * 3 + 1] = verticesY[i];
			m_Vertices[i * 3 + 2] = verticesZ[i];
			m_Colores[i * 3] = R;
			m_Colores[i * 3 + 1] = G;
			m_Colores[i * 3 + 2] = B;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
		glBufferData(
		    GL_ARRAY_BUFFER,
		    sizeof(m_Vertices),
			m_Vertices,
		    GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
			glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(m_Colores),
			m_Colores,
			GL_STATIC_DRAW);
}


void GenerarNumerosReloj(GLfloat Radio, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B){
		GLfloat radius = Radio;
		GLint numeroLados = 60;
		numeroVertices = numeroLados+2;
		GLfloat twicePi = 2.0f * M_PI;

		GLfloat verticesX[numeroVertices];
		GLfloat verticesY[numeroVertices];
		GLfloat verticesZ[numeroVertices];

		verticesX[0] = x;
		verticesY[0] = y;
		verticesZ[0] = z;
		for (int i = 0; i < numeroVertices; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numeroLados));
			verticesY[i] = y + (radius * sin(i * twicePi / numeroLados));
			verticesZ[i] = z;
		}
		GLint dimVertices = (numeroVertices) * 3;
		GLfloat m_Vertices[dimVertices];
		GLfloat m_Colores[dimVertices];


		for (int i = 0; i < numeroVertices; i++) {
			m_Vertices[i * 3] = verticesX[i];
			m_Vertices[i * 3 + 1] = verticesY[i];
			m_Vertices[i * 3 + 2] = verticesZ[i];
			m_Colores[i * 3] = R;
			m_Colores[i * 3 + 1] = G;
			m_Colores[i * 3 + 2] = B;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
		glBufferData(
		    GL_ARRAY_BUFFER,
		    sizeof(m_Vertices),
			m_Vertices,
		    GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
			glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(m_Colores),
			m_Colores,
			GL_STATIC_DRAW);

}
//--------------------------------------------------------------------------------
void GenerarRectangulo(GLfloat Altura, GLfloat Base, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B){
	GLfloat m_Vertices[18]={
		x, y, z,
		x, y+Altura, z,
		x+Base, y, z,
		x+Base, y, z,
		x+Base, y+Altura, z,
		x, y+Altura, z,
	};
	numeroVertices=6;
	GLfloat m_Colores[18]={
		R,G,B,
		R,G,B,
		R,G,B,
		R,G,B,
		R,G,B,
		R,G,B,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Vertices),
		m_Vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Colores),
		m_Colores,
		GL_STATIC_DRAW);
}

//--------------------------------------------------------------------------------
void GenerarTriangulo(GLfloat Altura, GLfloat Base, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B){
	GLfloat m_Vertices[9]={
		x, y, z,
		x+Base/2, y+Altura, z,
		x+Base , y, z,
	};
	numeroVertices=3;
	GLfloat m_Colores[9]={
		R,G,B,
		R,G,B,
		R,G,B,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Vertices),
		m_Vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Colores),
		m_Colores,
		GL_STATIC_DRAW);

}

void GenerarTriangulo1(GLfloat Altura, GLfloat Base, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B){
	GLfloat m_Vertices[9]={
		x, y, z,
		x+Altura, y-Base/2, z,
		x, y-Base, z,
	};
	numeroVertices=3;
	GLfloat m_Colores[9]={
		R,G,B,
		R,G,B,
		R,G,B,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Vertices),
		m_Vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Colores),
		m_Colores,
		GL_STATIC_DRAW);

}
//--------------------------------------------------------------------------------
void Animacion(glm::mat4 &model, int tipoAnimacion){
	switch(tipoAnimacion){
		case 0: break;
		case 1: //ANIMACION DE LA PELOTA
		{
			curAngle += 0.5f;
			paso += 0.001f;
			if (curAngle >= 360){ curAngle -= 360; }

			if (direction) {
				triOffset += triIncrement;
			} else {
				triOffset -= triIncrement;
			}

			if (std::abs(triOffset) >= triMaxOffset) {
				direction = !direction;
				paso = 1.0f;
			}
			if(triOffset <= -5.0){
				direction = !direction;
				paso = 1.0f;
			}

			glm::mat4 identidad(1.0f);
			glm::mat4 trasl = glm::translate(identidad, glm::vec3(95.0/100, 80.0/100, 0.0f));
			glm::mat4 traslinv = glm::translate(identidad, glm::vec3(-95.0/100, -80.0/100, 0.0f));
			glm::mat4 rot1 = glm::rotate(identidad, -curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 escalador = glm::scale(identidad,glm::vec3(paso,paso, 1.0f));//el escalamiento no se divide entre /100
			glm::mat4 Movimiento = glm::translate(identidad,glm::vec3(triOffset/100,0.0f, 1.0f));
			 model = traslinv * Movimiento * trasl  * traslinv *escalador* trasl * traslinv * rot1 * trasl *model;

			//std::cout << glm::to_string(model) << std::endl;
		}break;
		case 2:{//ROTACION SOBRE EL CENTRO DEL RELOJ , SOLO SEGUNDERO
			glm::mat4 identidad(1.0f);
			AnguloSegundero += 0.025f;//tabien
			//AnguloSegundero += 2.5f;
			if (AnguloSegundero >= 360){ AnguloSegundero -= 360; }
			glm::mat4 trasl = glm::translate(identidad, glm::vec3(0.0/100, -18.0/100, 0.0f));
			glm::mat4 traslinv = glm::translate(identidad, glm::vec3(0.0/100, 18.0/100, 0.0f));
			glm::mat4 rot1 = glm::rotate(identidad, -AnguloSegundero * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = traslinv * rot1 * trasl *model;
		}
		break;
		case 3:{//ANIMACION DEL PENDULO
			rotaPendulo=rotaPendulo+0.02;
			glm::mat4 identidad(1.0f);
			glm::mat4 trasl = glm::translate(identidad, glm::vec3(0.0/100, 12.0/100, 0.0f));
			glm::mat4 traslinv = glm::translate(identidad, glm::vec3(0.0/100, -12.0/100, 0.0f));
			glm::mat4 rot1 = glm::rotate(identidad, 45*sin(rotaPendulo) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = traslinv * rot1 * trasl *model;
		}
		break;

		case 4:{//ROTACION DEL MINUTERO
			glm::mat4 identidad(1.0f);
			glm::mat4 trasl = glm::translate(identidad, glm::vec3(0.0/100, -18.0/100, 0.0f));
			glm::mat4 traslinv = glm::translate(identidad, glm::vec3(0.0/100, 18.0/100, 0.0f));
			glm::mat4 rot1;
			if (AnguloSegundero == 0){
				AnguloMinutero = AnguloMinutero + 3;
				rot1 = glm::rotate(identidad, -AnguloMinutero * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}else{
				rot1 = glm::rotate(identidad, -AnguloMinutero * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			model = traslinv * rot1 * trasl *model;
		}
		break;



	}
}
//--------------------------------------------------------------------------------
void draw (int Mode_Type,int tipoAnimacion){
    // Use our shader
    glUseProgram(programID);

    GLuint uniformModel = glGetUniformLocation(programID, "model");
    glm::mat4 model = glm::ortho(-100.0f, 100.0f, -100.0f,100.0f);
    Animacion(model,tipoAnimacion);
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
    glVertexAttribPointer(
                          1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                          3,                                // size
                          GL_FLOAT,                         // type
                          GL_FALSE,                         // normalized?
                          0,                                // stride
                          (void*)0                          // array buffer offset
                          );
    GLenum mode[7] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP };
    glDrawArrays(mode[Mode_Type], 0, numeroVertices);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
void ReiniciarPuntero(){
	VBO_Puntero=0;
	CBO_Puntero=0;
}
void SumarPosicionPuntero(){
	VBO_Puntero++;
	CBO_Puntero++;
}

//--------------------------------------------------------------------------------
void DibujoICuadrante(){
	m_VBO= new GLuint[20];
	m_CBO= new GLuint[20];
	glBindVertexArray(m_VAO[0]);
	glGenBuffers(20, m_VBO);
	glGenBuffers(20, m_CBO);
	GenerarRectangulo(95.0, 60.0, -30.0, -55.0, 0.0,0.502, 0.251, 0.0);//el cuerpo
	draw(5,0);
	//SumarPosicionPuntero();
	GenerarRectangulo(95.0, 60.0, -30.0, -55.0, 0.0,0.0, 0.0, 0.0);
	draw(1,0);
	SumarPosicionPuntero();
	GenerarRectangulo(40.0, 40.0, -20.0, -52.0, 0.0, 0.602, 0.351, 0.0);//el cuadrado interior
	draw(5,0);
	SumarPosicionPuntero();
	GenerarTriangulo(20.0, 80.0, -40.0, 40.0, 0.0, 0.502, 0.251, 0.0);//el triangulo del techo
	draw(5,0);
	SumarPosicionPuntero();
	GenerarRectangulo(5.0, 80.0, -40.0, -60.0, 0.0, 0.502, 0.251, 0.0);//base del reloj
	draw(5,0);
	SumarPosicionPuntero();
	GenerarRectangulo(4.0, 60.0, -30.0, -8.0, 0.0, 0.402, 0.151, 0.0);//divisor entre reloj y pendulo
	draw(5,0);
	SumarPosicionPuntero();
	GenerarCirculo(19.74, 0.0, 18.0, 0.0, 0.402, 0.151, 0.0);//circulo mayor reloj
	draw(5,0);
	SumarPosicionPuntero();
	GenerarCirculo(15.59, 0.0, 18.0, 0.0, 0.302, 0.151, 0.0);//circulo intermedio reloj
	draw(5,0);
	SumarPosicionPuntero();
	GenerarCirculo(13.217, 0.0, 18.0, 0.0, 1.0, 1.0, 1.0);//circulo menor reloj
	draw(5,0);
	SumarPosicionPuntero();
	GenerarRectangulo(18.0, 0.5, 0.0, -30.0, 0.0,0.0, 0.0, 0.0);//cuerda del pendulo
	draw(5,3);
	SumarPosicionPuntero();
	GenerarCirculo(5.0, 0.0, -35.0, 0.0, 0.192, 0.192, 0.192);//circulo del pendulo
	draw(5,3);
	SumarPosicionPuntero();
	GenerarCirculo(1.0, 0.0, 18.0, 0.0, 0.0, 0.0, 0.0);//centro del reloj
	draw(5,0);
	SumarPosicionPuntero();
	GenerarRectangulo(7.0, 0.5, -0.0, 19.0, 0.0,0.0, 0.0, 0.0);//minutero
	draw(5,4);
	SumarPosicionPuntero();
	GenerarTriangulo(1.0, 2.0, -1.0, 26.0, 0.0, 0.0, 0.0, 0.0);//ajuga del minutero
	draw(5,4);
	SumarPosicionPuntero();
	GenerarRectangulo(0.5, 9.0, 1.0, 18.0, 0.0,0.0, 0.0, 0.0);//segundero
	draw(5,2);
	SumarPosicionPuntero();
	GenerarTriangulo1(1.0, 2.0, 10.0, 19.25, 0.0, 0.0, 0.0, 0.0);//ajuga del segundero
	draw(5,2);
	SumarPosicionPuntero();
	GenerarCirculo(5.0, -95.0, -80.0, 0.0, 0.73, 0.11, 0.027);//rueda
	draw(5,1);

	ReiniciarPuntero();
}

//--------------------------------------------------------------------------------
void transferDataToGPUMemory(int i){
	programID = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
	glGenVertexArrays(4, m_VAO);
	switch(i){
		case 1:
			DibujoICuadrante();
			break;
	}
}

//--------------------------------------------------------------------------------

void cleanupDataFromGPU(){
	glDeleteBuffers(1, m_VBO);
	glDeleteBuffers(1, m_CBO);
	glDeleteVertexArrays(1, m_VAO);
	glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------
int main( void )
{
    // Inicializar GLFW
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Abrir una ventana
    window = glfwCreateWindow(800, 800, "Tarea 07: Transformaciones", NULL, NULL);

    // Crear un contexto de Windows
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = true; // Needed for core profile
    glewInit();

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    //glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // render scene for each frame
    do{
        // White background
        glClearColor(62.0f/255.0f, 229.0f/255.0f, 166.0f/255.0f, 0.46f);

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        transferDataToGPUMemory(1);

        // Swap buffers
        glfwSwapBuffers(window);
        // looking for events
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );

    // delete framebuffer
    glDeleteFramebuffers(1, &FramebufferName);

    // Cleanup VAO, VBOs, and shaders from GPU
    cleanupDataFromGPU();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

