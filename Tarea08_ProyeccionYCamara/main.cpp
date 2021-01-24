//============================================================================
// Name        : Tarea08_Proyeccion y Camara Grupo 01
// Professor   : Herminio Paucar
// Alumnos     : N�stor S�nchez, Juan Diego C�rdova, Rosinaldo Rivera
// Description : Gunbound en 3D
//============================================================================


/* imgui */
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

/* graphics header */
#include <GL\glew.h>
#include <GLFW\glfw3.h>

/* math */
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

/* basic headers */
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
/* own headers */
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#ifndef MPI
#define M_PI 3.14159265358979323846
#endif

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
float xpos; float ypos;
bool firstMouse = true;
float yawXD   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitchXD =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

static float cameraX=0.0f, cameraY=0.0f, cameraZ=0.0f;
static float RotacionCameraX=0.0f, RotacionCameraY=0.0f, RotacionCameraZ=0.0f;
int width, height;
float aspect;
//glm::mat4 pMat, mvMat;
const float DEG2RAD = acos(-1) / 180.0f;  // PI/180
int dimension;

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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback()
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yawXD = xoffset+yawXD;
    pitchXD = yoffset+pitchXD;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitchXD > 89.0f)
        pitchXD = 89.0f;
    if (pitchXD < -89.0f)
        pitchXD = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yawXD)) * cos(glm::radians(pitchXD));
    front.y = sin(glm::radians(pitchXD));
    front.z = sin(glm::radians(yawXD)) * cos(glm::radians(pitchXD));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void processInput(GLFWwindow *window)
{
	float currentFrame = glfwGetTime();
	        deltaTime = currentFrame - lastFrame;
	        lastFrame = currentFrame;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 20 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
//---------------------------------------------------------------------------------------------------
void RellenarBufferDatosyColores(float (*m_Vertices),float (*m_Colores), int tamVertices,int tamColores){
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VBO_Puntero]);
			glBufferData(
			    GL_ARRAY_BUFFER,
				tamVertices,
				m_Vertices,
			    GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_CBO[CBO_Puntero]);
				glBufferData(
				GL_ARRAY_BUFFER,
				tamColores,
				m_Colores,
				GL_STATIC_DRAW);

}

//---------------------------------------------------------------------------------------------------
void GenerarCirculo(GLfloat Radio, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B, int plano){

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
		switch(plano){
			case 0://XY
				for (int i = 0; i < numeroVertices; i++) {
					verticesX[i] = x + (radius * sin(i * twicePi / numeroLados));
					verticesY[i] = y + (radius * cos(i * twicePi / numeroLados));
					verticesZ[i] = z;
				}
				break;
			case 1://XZ
				for (int i = 0; i < numeroVertices; i++) {
					verticesX[i] = x + (radius * sin(i * twicePi / numeroLados));
					verticesY[i] = y ;
					verticesZ[i] = z+ (radius * cos(i * twicePi / numeroLados));
				}
				break;
			case 2://YZ
				for (int i = 0; i < numeroVertices; i++) {
					verticesX[i] = x ;
					verticesY[i] = y + (radius * sin(i * twicePi / numeroLados));
					verticesZ[i] = z+ (radius * cos(i * twicePi / numeroLados));
				}
				break;
		}

		GLint dimVertices = (numeroVertices) * 3;
		float m_Vertices[dimVertices];
		float m_Colores[dimVertices];


		for (int i = 0; i < numeroVertices; i++) {
			m_Vertices[i * 3] = verticesX[i];
			m_Vertices[i * 3 + 1] = verticesY[i];
			m_Vertices[i * 3 + 2] = verticesZ[i];
			m_Colores[i * 3] = R;
			m_Colores[i * 3 + 1] = G;
			m_Colores[i * 3 + 2] = B;
		}

		RellenarBufferDatosyColores(m_Vertices,m_Colores,sizeof(m_Vertices),sizeof(m_Colores));

}

void GenerarSemiCirculo(GLfloat Radio, GLfloat x, GLfloat y, GLfloat z,GLuint A,GLfloat R, GLfloat G , GLfloat B,int plano){

		GLfloat radius = Radio;
		GLint numeroLados = 100;
		numeroVertices = (numeroLados+2)/2;
		GLfloat twicePi = 2.0f * M_PI;
		GLfloat verticesX[numeroVertices];
		GLfloat verticesY[numeroVertices];
		GLfloat verticesZ[numeroVertices];

		int aux=(numeroLados)*A/360;
		switch(plano){
					case 0://XY
						verticesX[0] = (radius * cos(A * twicePi / 360));
						verticesY[0] = (radius * sin(A * twicePi/ 360));
						verticesZ[0] = z;
						for (int i=aux-1; i <aux+numeroVertices; i++) {
							verticesX[i-(aux)] = x + (radius * cos(i * twicePi / numeroLados));
							verticesY[i-(aux)] = y + (radius * sin(i * twicePi / numeroLados));
							verticesZ[i-(aux)] = z;
						}
						break;
					case 1://XZ
						verticesX[0] = (radius * sin(A * twicePi/ 360));
						verticesY[0] = y;
						verticesZ[0] = (radius * cos(A * twicePi / 360));
						for (int i=aux-1; i <aux+numeroVertices; i++) {
							verticesX[i-(aux)] = x + (radius * sin(i * twicePi / numeroLados));
							verticesY[i-(aux)] = y ;
							verticesZ[i-(aux)] = z + (radius * cos(i * twicePi / numeroLados));
						}
						break;
					case 2://YZ
						verticesX[0] = x;
						verticesY[0] = (radius * cos(A * twicePi / 360));
						verticesZ[0] = (radius * sin(A * twicePi/ 360));
						for (int i=aux-1; i <aux+numeroVertices; i++) {
							verticesX[i-(aux)] = x ;
							verticesY[i-(aux)] = y + (radius * cos(i * twicePi / numeroLados));
							verticesZ[i-(aux)] = z + (radius * sin(i * twicePi / numeroLados));
						}
						break;
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

		RellenarBufferDatosyColores(m_Vertices,m_Colores,sizeof(m_Vertices),sizeof(m_Colores));
}

//--------------------------------------------------------------------------------
void GenerarRectangulo(GLfloat Altura, GLfloat Base, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B,int plano){
	GLfloat p1,p2,p3,p4,p5,p6,p7,p8,p9;
	switch(plano){
		case 0:
			p1=x + Base ;p2=y		  ;p3=z ;
			p4=x + Base ;p5=y - Altura;p6=z ;
			p7=x  		;p8=y - Altura;p9=z ;
			break;
		case 1:
			p1=x + Base ;p2=y		  ;p3=z ;
			p4=x + Base ;p5=y		  ;p6=z + Altura;
			p7=x  		;p8=y 		  ;p9=z + Altura ;
			break;
		case 2:
			p1=x 		;p2=y		  ;p3=z + Altura;
			p4=x 		;p5=y - Altura;p6=z+ Altura ;
			p7=x  		;p8=y - Altura;p9=z ;
			break;
	}

	GLfloat m_Vertices[18]={
		x, y, z,
		p1,p2,p3,
		p7,p8,p9,
		p4,p5,p6,
		p1,p2,p3,
		p7,p8,p9
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
	RellenarBufferDatosyColores(m_Vertices,m_Colores,sizeof(m_Vertices),sizeof(m_Colores));
}

//--------------------------------------------------------------------------------
void GenerarTriangulo(GLfloat Altura, GLfloat Base, GLfloat x, GLfloat y, GLfloat z,GLfloat R, GLfloat G , GLfloat B,int plano){
	GLfloat p1,p2,p3,p7,p8,p9;
		switch(plano){
			case 0:
				p1=x + Base/2 ;p2=y		  ;p3=z ;
				p7=x  		;p8=y - Altura;p9=z ;
				break;
			case 1:
				p1=x + Base/2 ;p2=y		  ;p3=z ;
				p7=x  		;p8=y 		  ;p9=z + Altura ;
				break;
			case 2:
				p1=x 		;p2=y		  ;p3=z + Altura;
				p7=x  		;p8=y - Altura;p9=z ;
				break;
		}
	GLfloat m_Vertices[9]={
		x, y, z,
		p1,p2,p3,
		p7,p8,p9
	};
	numeroVertices=3;
	GLfloat m_Colores[9]={
		R,G,B,
		R,G,B,
		R,G,B,
	};
	RellenarBufferDatosyColores(m_Vertices,m_Colores,sizeof(m_Vertices),sizeof(m_Colores));
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
	RellenarBufferDatosyColores(m_Vertices,m_Colores,sizeof(m_Vertices),sizeof(m_Colores));
}



//--------------------------------------------------------------------------------
void Animacion(glm::mat4 &model, int tipoAnimacion){
	glm::mat4 identidad(1.0f);
	//glfwGetTime ();
	switch(tipoAnimacion){
		case 0: break;
		case 1:
			{ glm::mat4 trasl = glm::translate(identidad, glm::vec3(10.0f,-10.0f, 0.0f));
			glm::mat4 traslinv = glm::translate(identidad, glm::vec3(-10.0f,10.0f, 0.0f));
			  glm::mat4 rotacion = glm::rotate(identidad, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
				model =  traslinv * rotacion * trasl *model;

			}
			break;
		case 2:
			break;
		case 3:
			break;

		case 4:
			break;



	}
}
//--------------------------------------------------------------------------------
void draw (int Mode_Type,int tipoAnimacion){
    // Use our shader
    glUseProgram(programID);
    GLuint modelLoc = glGetUniformLocation(programID, "model_matrix");
    GLuint projLoc = glGetUniformLocation(programID, "proj_matrix");
    GLuint viewLoc = glGetUniformLocation(programID, "view_matrix");
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float) width / (float) height;
    //PROYECTION
    glm::mat4 pMat;
    pMat = glm::perspective(glm::radians(60.0f), aspect, 1.0f, 100000.0f); // 1.0472 radians == 60 degrees
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    //MODEL
    glm::mat4 pModel;
    glm::mat4 trasl = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    glm::mat4 rotamvMatX = glm::rotate(  glm::mat4(1.0f), RotacionCameraX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotamvMatY = glm::rotate(  glm::mat4(1.0f), RotacionCameraY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotamvMatZ = glm::rotate(  glm::mat4(1.0f), RotacionCameraZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    pModel = rotamvMatZ*rotamvMatY*rotamvMatX*trasl;
    Animacion(pModel,tipoAnimacion);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pModel));
    //VIEW
    glm::mat4 pView;
    pView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(pView));



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
//El Cubo se puede personalizar segun el color de un lado, EL ORIGEN NO SE CAMBIA
void GenerarCubo(glm::vec3 origen,int tamanio,glm::vec3 color){
	//Lado 1
	GenerarRectangulo(tamanio,tamanio,origen.x, origen.y, origen.z,0.0f, color.y , color.z,0);//XY
	draw(5,0);
	//Lado 2
	GenerarRectangulo(tamanio,tamanio,origen.x, origen.y, origen.z,0.8f, color.y , color.z,1);//XZ
	draw(5,0);
	GenerarRectangulo(tamanio,tamanio,origen.x, origen.y, origen.z,0.9f, color.y , color.z,2);//YZ
	draw(5,0);
	//Lado 3
	GenerarRectangulo(tamanio,tamanio,origen.x+tamanio, origen.y, origen.z,color.x, color.y , color.z,2);//YZ
	draw(5,0);
	//Lado 4
	GenerarRectangulo(tamanio,tamanio,origen.x, origen.y-tamanio, origen.z,color.x, color.y , color.z,1);//YZ
	draw(5,0);
	//lado 5
	GenerarRectangulo(tamanio,tamanio,origen.x, origen.y, origen.z+tamanio,color.x, color.y , color.z,0);//XY
	draw(5,0);

}

//--------------------------------------------------------------------------------
void DibujoICuadrante(){
	m_VBO= new GLuint[20];
	m_CBO= new GLuint[20];
	glBindVertexArray(m_VAO[0]);
	glGenBuffers(20, m_VBO);
	glGenBuffers(20, m_CBO);

	GenerarCubo(glm::vec3(10.0f,-10.0f,-50.0f),5,glm::vec3(1.0f,0.234f,0.486f));


	/*--[FRAME IMGUI]-----------*/
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	/*--[FRAME IMGUI]-----------*/



	/*--[CODIGO IMGUI]-----------*/
	ImGui::Begin("Panel de Control de la Camara"); // Aqui inicia una peque�a ventana
	ImGui::PushStyleColor(ImGuiCol_Button, {209.0f/255.0f, 116.0f/255.0f, 11.0f/255.0f, 1} );
	 bool ResetMovi = ImGui::Button("Resetear Movimiento");
	bool ResetRota =ImGui::Button("Resetear Rotacion");
	bool ResetVista =ImGui::Button("Resetear VISTA");
	ImGui::PopStyleColor(1);
	if(ResetMovi){
		cameraX =0.0f;
		cameraY =0.0f;
		cameraZ =0.0f;
	}
	if(ResetRota){
		RotacionCameraX =0.0f;
		RotacionCameraY =0.0f;
		RotacionCameraZ =0.0f;
	}
	if(ResetVista){
	    yawXD   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		pitchXD =  0.0f;
		cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
		xpos=0.0f;
		ypos=0.0f;
	}
	ImGui::Text("Movimiento de la Camara");
	ImGui::SliderFloat("M. Camara X", &cameraX, -360, 360);
	ImGui::SliderFloat("M. Camara y", &cameraY, -360, 360);
	ImGui::SliderFloat("M. Camara Z", &cameraZ, -360, 360);
	ImGui::Text("Rotacion de la Camara");
	ImGui::SliderFloat("R. Camara X", &RotacionCameraX, -360, 360);
	ImGui::SliderFloat("R. Camara Y", &RotacionCameraY, -360, 360);
	ImGui::SliderFloat("R. Camara Z", &RotacionCameraZ, -360, 360);
	ImGui::Text("VISTA");
	ImGui::SliderFloat("VISTA X", &xpos, -720, 720);
	ImGui::SliderFloat("VISTA Y", &ypos, -720, 720);
	 mouse_callback();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Muestra los FPS de la aplicaci�n
	ImGui::End(); // Aqui termina la ventana ultima en iniciar
	/*--[CODIGO IMGUI]-----------*/

	/*--[RENDER IMGUI]-----------*/
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	/*--[RENDER IMGUI]-----------*/


	ReiniciarPuntero();


}

//--------------------------------------------------------------------------------
void transferDataToGPUMemory(int i){
	programID = createShaderProgram("vertShader.glsl", "fragShader.glsl");
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
    window = glfwCreateWindow(1600, 1600, "Tarea 08: Proyeccion y Camara", NULL, NULL);

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
    /*--[INICIALIZACION IMGUI]-----------*/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	/*--[INICIALIZACION IMGUI]-----------*/
    // render scene for each frame
    do{
        // White background
        glClearColor(62.0f/255.0f, 229.0f/255.0f, 166.0f/255.0f, 0.46f);

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        glClear(GL_DEPTH_BUFFER_BIT);
        // Habilidad el test de profundidad
        glEnable(GL_DEPTH_TEST);
        // Aceptar el fragmento si est� m�s cerca de la c�mara que el fragmento anterior
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        processInput(window);
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

    /*--[CLEAN IMGUI]-----------*/
       ImGui_ImplOpenGL3_Shutdown();
       ImGui_ImplGlfw_Shutdown();
       ImGui::DestroyContext();
       /*--[CLEAN IMGUI]-----------*/

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

