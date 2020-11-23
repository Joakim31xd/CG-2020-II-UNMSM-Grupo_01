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

/* basic headers */
#include <iostream>

/* own headers */
#include "utils.h"

GLFWwindow *window;
GLuint vao, vbo,cbo;
GLuint program;
GLint numeroVertices;

float vertices [] = {
    -.5f, -.5f, .0f,
     .5f, -.5f, .0f,
     .0f,  .5f, .0f
};
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

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
		    GL_ARRAY_BUFFER,
		    sizeof(m_Vertices),
			m_Vertices,
		    GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, cbo);
			glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(m_Colores),
			m_Colores,
			GL_STATIC_DRAW);

}
void init(GLFWwindow *window){

    /*--[INICIALIZACION IMGUI]-----------*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    /*--[INICIALIZACION IMGUI]-----------*/

    program = createShaderProgram("vshader.glsl", "fshader.glsl" );
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    //glClearColor(235.0f/255.0f,223.0f/255.0f,190.0f/255.0f, 1.0);

    /*--[FRAME IMGUI]-----------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    /*--[FRAME IMGUI]-----------*/
    bool show_another_window = false;

    static float tras_x = 0, tras_y=0;
    static float rota_x = 0, rota_y = 0;
    static float scale = 1;
    static float Objectred = 0,Objectblue = 0,Objectgreen = 0;
    static float Backgroundred = 0,Backgroundblue = 0,Backgroundgreen = 0;

    //static const ImVec4 pressColor{ 0.0f, 0.217f, 1.0f, 0.784f };
    /*--[CODIGO IMGUI]-----------*/
    ImGui::Begin("Panel de Control"); // Aqui inicia una pequeña ventana
    ImGui::PushStyleColor(ImGuiCol_Button, {166.0f/255.0f, 77.0f/255.0f, 121.0f/255.0f, 1} );
    bool Left=ImGui::Button("Move Left");ImGui::SameLine();
	bool Up=ImGui::Button("Move Up");ImGui::SameLine();
	bool Rigth=ImGui::Button("Move Rigth");
	bool Down=ImGui::Button("Move Down");
	ImGui::PopStyleColor(1);

	if (Rigth){
		tras_x=tras_x+0.08;
	}else if(Left){
		tras_x=tras_x-0.08;
	}else if(Up){
		tras_y=tras_y+0.08;
	}else if(Down){
		tras_y=tras_y-0.08;
	}

	if(ImGui::Button("Change Object Color")){
		if (show_another_window==false)
		        {
		            ImGui::Begin("Change Object Color", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		            ImGui::Text("Hello from another window!");
		            if (ImGui::Button("Close Me"))
		                show_another_window = false;
		        }
	}

	ImGui::Text("Color Red");
	ImGui::SliderFloat("RED", &Objectred, 0, 255);
	ImGui::Text("Color Green");
	ImGui::SliderFloat("GREEN", &Objectgreen,0, 255);
	ImGui::Text("Color Blue");
	ImGui::SliderFloat("BLUE", &Objectblue, 0, 255);


	ImGui::Text("Background Red");
	ImGui::SliderFloat("B-RED", &Backgroundred, 0, 255);
	ImGui::Text("Background Green");
	ImGui::SliderFloat("B-GREEN", &Backgroundgreen,0, 255);
	ImGui::Text("Background Blue");
	ImGui::SliderFloat("B-BLUE", &Backgroundblue, 0, 255);


/*
	ImGui::Text("Traslaciones");
    ImGui::SliderFloat("tras. x", &tras_x, -1, 1);
    ImGui::SliderFloat("tras. y", &tras_y, -1, 1);
    ImGui::Text("Rotaciones");
	ImGui::SliderFloat("rota. x", &rota_x, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::SliderFloat("rota. y", &rota_y, -3.14 * 2.0, 3.14 * 2.0);
	ImGui::Text("Escalamiento");
	ImGui::SliderFloat("scala", &scale, 0, 4);*/
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Muestra los FPS de la aplicación

    ImGui::End(); // Aqui termina la ventana ultima en iniciar
    /*--[CODIGO IMGUI]-----------*/


    glUseProgram(program);
    glBindVertexArray(vao);

    glClearColor(Backgroundred/255.0f,Backgroundgreen/255.0f,Backgroundblue/255.0f, 1.0);
    //color
    GLint color_location = glGetUniformLocation(program, "colorxd");
    GLfloat color[3] = {Objectred/255.0f, Objectgreen/255.0f, Objectblue/255.0f};
    glUniform3fv(color_location, 1.0f, color);
    //fin_color


    glm::mat4 trans = translate(glm::mat4(1), glm::vec3(tras_x, tras_y, 0));
    trans = glm::rotate(trans, rota_x, glm::vec3(1, 0, 0));
    trans = glm::rotate(trans, rota_y, glm::vec3(0, 1, 0));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1));
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation_matrix"), 1, false, value_ptr(trans));

    glDrawArrays(GL_TRIANGLES, 0, 3);



    /*--[RENDER IMGUI]-----------*/
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /*--[RENDER IMGUI]-----------*/


}

int main(){
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Tarea 05: Shaders y IMGUI", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    if (glewInit() != GLEW_OK) return 1;

    init(window);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        display();
        glfwSwapBuffers(window);
    }

    /*--[CLEAN IMGUI]-----------*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    /*--[CLEAN IMGUI]-----------*/

}
