#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

#include "../SOIL2/SOIL2.h"
#include "../include/Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>
#include <stack>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "Sphere.h"

using namespace std;
using namespace cv;

const float arucoSquareDimension = 0.05f;
Sphere mySphere = Sphere(48);
GLuint VBO, VAO;
GLuint mvLoc, projLoc,modelLoc;
stack<glm::mat4> mvPila;
int width = 640, height = 800,width2,height2;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void DibujarEsfera(){
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();
	int numIndices = mySphere.getNumIndices();
	std::vector<float> ValoresEsfera;

	for (int i = 0; i < numIndices; i++) {
		ValoresEsfera.push_back((vert[ind[i]]).x);
		ValoresEsfera.push_back((vert[ind[i]]).y);
		ValoresEsfera.push_back((vert[ind[i]]).z);
		ValoresEsfera.push_back((tex[ind[i]]).s);
		ValoresEsfera.push_back((tex[ind[i]]).t);
		ValoresEsfera.push_back((norm[ind[i]]).x);
		ValoresEsfera.push_back((norm[ind[i]]).y);
		ValoresEsfera.push_back((norm[ind[i]]).z);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ValoresEsfera.size()*4, &ValoresEsfera[0], GL_STATIC_DRAW);

	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Normal Attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(5* sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

int main()
{
	// OpenCV
	cv::Mat rot_mat;

	// GLFW ====================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Aplicacion Aruco", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, 800, 600);

	//Creacion de los Shaders
	GLuint renderingProgram = Utils::createShaderProgram("src/shaders/vertShader.glsl", "src/shaders/fragShader.glsl");
	GLuint bgProgram = Utils::createShaderProgram("src/shaders/bgVertShader.glsl", "src/shaders/bgFragShader.glsl");
	GLuint LampProgram = Utils::createShaderProgram("src/shaders/lampVertShader.glsl", "src/shaders/lampFragShader.glsl");

	/*
	// Camera Matrix & Distortion Parameters
	cv::Mat intrinsic_matrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distortion_parameters;
	loadCameraCalibration("CAMARA", intrinsic_matrix, distortion_parameters);*/

	//PARAMETROS DESIGNADOS PARA LA CAMARA
	double Ofx, Ofy, Ocx, Ocy, Ok1, Ok2, Ok3, Op1, Op2;
	Ofx = 955.8925;
	Ofy = 955.4439;
	Ocx = 296.9006;
	Ocy = 215.9074;
	Ok1 = -0.1523;
	Ok2 = 0.7722;
	Ok3 = 0;
	Op1 = 0;
	Op2 = 0;
	cv::Mat intrinsic_matrix = (cv::Mat_<float>(3, 3) <<
		Ofx, 0.0, Ocx,
		0.0, Ofy, Ocy,
		0.0, 0.0, 1.0);
	cv::Mat distortion_parameters = (cv::Mat_<float>(5, 1) << Ok1, Ok2, Op1, Op2, Ok3);


	// Video Variables
	cv::Mat frame;
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	cv::aruco::DetectorParameters parameters;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_250);
	cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 5, 0.04, 0.01, markerDictionary);

	//------------------ASIGNACION DE CAMARA SEA UNA IP DE CELULAR O CAMARA INTEGRADA/EXTERNA DE LA COMPUTADORA---------------
	//camara por droidcam
	//cv::VideoCapture vid("http://192.168.1.5:4747/video");
	//camara de laptop o pc
	cv::VideoCapture vid(0);
	if (!vid.isOpened()) {
		return -1;
	}

	cv::Mat rotationVectors, translationVectors;
	//FUNCION QUE ASIGNA LOS VALORES DE POSICION,TEXTURA Y NORMAL DE UNA ESFERA
	DibujarEsfera();

	// ASIGNACION DE IMAGENES A TEXTURA
	GLuint texture1, texture2;GLuint texture3; // para el sol, la tierra y la luna

	///////////////////////////////////////////////////////////////////////////////// TEXTURA DEL SOL
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;

	unsigned char* image1 = SOIL_load_image("resources/sun.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	/////////////////////////////////////////////////////////////////////////////////TEXTURA DE LA TIERRA

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("resources/earth.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	///////////////////////////////////////////////////////////////////////////////////TEXTURA LUNA

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image3 = SOIL_load_image("resources/moon.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image3);
	glBindTexture(GL_TEXTURE_2D, 0);

	// =======================================================================
	// scaling matrix for the 3d object
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	// default values of modelview so that initially when there is no marker, the object is placed behind the camera and it does not show up
	glm::mat4 modelview = glm::mat4(1.0f);
	modelview = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));
	modelview = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));

	// Ambient light
	glm::vec3 light_color;
	glm::vec3 ambient_light;
	float ambient_strength;
	ambient_strength = 0.2f;
	light_color = glm::vec3(255.0f / 255.0f, 219.0f / 255.0f, 58.0f / 255.9f);
	ambient_light = ambient_strength * light_color;

	// defining the projection matrix,
	float near = 0.2f;
	float far = 400.0f;
	float fx = 955.8925;
	float fy = 955.4439;
	float cx = 296.9006;
	float cy = 215.9074;

	// OpenCV -> OpenGL perspective projection matrix generation
	glm::mat4 projection_perspective = { fx / cx,                  0,                   0,                                   0,
											0,                  fy / cy,                0,                                   0,
											0,                     0,     -(far + near) / (far - near),     -(2 * far * near) / (far - near),
											0,                     0,                  -1,                                   0 };

	// OpenGL reads matrices in column major order so we need to transpose projection perspective

	projection_perspective = glm::transpose(projection_perspective);
	//projection_perspective = glm::mat4(1.0f);


	// =========================================================================================================
	// background object definitions
	GLfloat vertices_bg[] =
	{
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	};

	// -----------------------------------------------------------------------------------------------
	// background object
	GLuint VBO_bg, VAO_bg;
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);

	glBindVertexArray(VAO_bg);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO_bg


	// -----------------------------------------------------------------------------------------------
	// webcam texture
	vid >> frame;
	width2 = frame.size().width;
	height2 = frame.size().height;

	GLuint texture_bg;
	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	// =========================================================================================================
	// transformation presets for the background object (model, view and orthographic projection)
	glm::mat4 modelview_bg = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -399.8, 0, 0, 0, 1 };
	modelview_bg = glm::transpose(modelview_bg);
	modelview_bg = glm::scale(modelview_bg, glm::vec3(798 * cx / fx, 798 * cy / fy, 0));
	modelview_bg = glm::translate(modelview_bg, glm::vec3(0.0f, 0.0f, 0.0f));


	// =========================================================================================================


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelview = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };

		vid >> frame;
		cv::aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);

		if (markerIds.size() > 0) {
			//Dibujar Markers
			//cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
			int valid = estimatePoseBoard(markerCorners, markerIds, board, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors);
			if (valid > 0) {
				cv::Rodrigues(rotationVectors, rot_mat);
				modelview = { rot_mat.at<double>(0,0),  rot_mat.at<double>(0,1),  rot_mat.at<double>(0,2),  translationVectors.at<double>(0),
							 -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -translationVectors.at<double>(1),
							 -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -translationVectors.at<double>(2),
									   0.0f,                      0.0f,                     0.0f,                   1.0f };
				modelview = glm::transpose(modelview);
				cv::aruco::drawAxis(frame, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors, 0.05f);
			}
		}
		//USAMOS LOS SHADERS DEL RENDERING PROGRAM
		glUseProgram(renderingProgram);
		mvLoc = glGetUniformLocation(renderingProgram, "modelview");
		projLoc = glGetUniformLocation(renderingProgram, "projection_perspective");
		modelLoc = glGetUniformLocation(renderingProgram, "model");

		modelview = glm::scale(modelview, glm::vec3(0.07f, 0.07f, 0.07f));
		modelview = glm::translate(modelview, glm::vec3(1.4f, 1.4f, 1.0f));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_perspective));

		//===================================================================================================================
		//MODELO EJEMPLO DEL SOL

		//POSICION DE LA LAMPARA
		glm::vec3 lamp_pos(1.0f, 1.0f, 0.0f);

		//TRANSFORMACIONES DEL SOL
		mvPila.push(modelview);
		mvPila.push(mvPila.top());
		mvPila.top() *= glm::scale(glm::mat4(1.0f),glm::vec3(1.5f, 1.5f, 1.5f));  // Escalamiento
		mvPila.push(mvPila.top());
		mvPila.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));  // Posicion del Sol
		mvPila.push(mvPila.top());  // duplicating
		mvPila.top() *= glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(),glm::vec3(1.0f, 0.0f, 0.0f));  // Rotacion del Sol

		//ASIGNACION DE TEXTURA PARA EL SOL
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//PARAMETROS ASIGNADOS A LOS SHADERS DEL SOL
		glUniform1i(glGetUniformLocation(renderingProgram, "Textura"), 0);
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvPila.top()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(renderingProgram, "ambient_light"),1,glm::value_ptr(ambient_light));
		glUniform3fv(glGetUniformLocation(renderingProgram, "lamp_pos"),1,glm::value_ptr(lamp_pos));
		glUniform3fv(glGetUniformLocation(renderingProgram, "light_color"),1,glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "sphere_color"),1,glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "camera_pos"),1,glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

		//ENLAZAMOS EL BUFFER CON EL VBO ESFERA
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Postion Attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Texture Attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(3* sizeof(float)));
		glEnableVertexAttribArray(1);
		// Normal Attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(5* sizeof(float)));
		glEnableVertexAttribArray(2);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());//DIBUJAR LA ESFERA
		mvPila.pop();//SACAMOS LA PILA

		//===================================================================================================================
		//MODELO DE LA TIERRA

		// Light position
		glm::vec3 lamp_pos2(sin(glfwGetTime() / 2.0f), cos(glfwGetTime() / 2.0f), 0.0f);

		//TRANSFORMACIONES DE LA TIERRA
		mvPila.push(mvPila.top());  // mvMat of EARTH
		mvPila.top() *= glm::scale(glm::mat4(1.0f),glm::vec3(0.5f, 0.5f, 0.5f));  // Escalamiento
		mvPila.push(mvPila.top());
		mvPila.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(sin((float) glfwGetTime()) * 4.0, 0.0f,cos((float) glfwGetTime()) * 4.0));  // planet position
		mvPila.push(mvPila.top());  // duplicating
		mvPila.top() *= glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(),glm::vec3(0.0, 1.0, 0.0));  // planet rotation

		//ASIGNACION DE TEXTURA PARA LA TIERRA
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//PARAMETROS ASIGNADOS A LOS SHADERS DE LA TIERRA
		glUniform1i(glGetUniformLocation(renderingProgram, "Textura"), 1);
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvPila.top()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(renderingProgram, "ambient_light"),1,glm::value_ptr(ambient_light));
		glUniform3fv(glGetUniformLocation(renderingProgram, "lamp_pos"),1,glm::value_ptr(lamp_pos2));
		glUniform3fv(glGetUniformLocation(renderingProgram, "light_color"),1,glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "sphere_color"),1,glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "camera_pos"),1,glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

		//ENLAZAMOS EL BUFFER CON EL VBO ESFERA
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Postion Attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Texture Attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(3* sizeof(float)));
		glEnableVertexAttribArray(1);
		// Normal Attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(5* sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());//Dibujar la esfera
		mvPila.pop();//SACAMOS LA PILA

		//===================================================================================================================
		//MODELO DE LA LUNA

		//TRANSFORMACIONES DE LA LUNA
		mvPila.push(mvPila.top());  // mvMat de luna
		mvPila.top() *= glm::translate(glm::mat4(1.0f),
		glm::vec3(0.0f, sin((float) glfwGetTime()) * 2.0, cos((float) glfwGetTime()) * 2.0));  // POSICION DE LA LUNA
		mvPila.push(mvPila.top());  // duplicating
		mvPila.top() *= glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(),glm::vec3(0.0, 0.0, 1.0));  // ROTACION DE LA LUNA
		mvPila.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));  //HACE LA LUNA MAS PEQUENA

		//ASIGNACION DE TEXTURA PARA LA LUNA
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3); //aplica la textura almacenada donde estaba la luna

		//PARAMETROS ASIGNADOS A LOS SHADERS DE LA LUNA
		glUniform1i(glGetUniformLocation(renderingProgram, "Textura"), 2);
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvPila.top()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(renderingProgram, "ambient_light"),1,glm::value_ptr(ambient_light));
		glUniform3fv(glGetUniformLocation(renderingProgram, "lamp_pos"),1,glm::value_ptr(lamp_pos2));
		glUniform3fv(glGetUniformLocation(renderingProgram, "light_color"),1,glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "sphere_color"),1,glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		glUniform3fv(glGetUniformLocation(renderingProgram, "camera_pos"),1,glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//ENLAZAMOS EL BUFFER CON EL VBO ESFERA
		//ATRIBUTOS DE POSICION
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (GLvoid*)0); /////////////// LOS 2 ULTIMOS PARAMETROS
		glEnableVertexAttribArray(0); ////////////// QUE NUM VA
		//ATRIBUTOS DE TEXTURA
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(3* sizeof(float)));
		glEnableVertexAttribArray(1);
		// Normal Attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float) , (GLvoid*)(5* sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());//Dibujar la esfera
		mvPila.pop();//SACAMOS LA PILA


		//Se elimina mvPila segun las veces que se ha creado objetos
		mvPila.pop();
		mvPila.pop();
		mvPila.pop();

		//===================================================================================================================
		// Draw BG , se usan los SHADERS de bgProgram
		glUseProgram(bgProgram);
		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE3);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bgProgram, "webcam_texture"), 3);

		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//===================================================================================================================
		// Draw lamp , se usan los SHADERS de LampProgram
		glm::mat4 model_lamp = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, lamp_pos.x, lamp_pos.y, 0.0f, 1.0f };
		model_lamp = glm::scale(model_lamp, glm::vec3(0.1f, 0.1f, 0.1f));
		glm::mat4 modelview_lamp = glm::mat4(1.0f);

		glUseProgram(LampProgram);
		glUniformMatrix4fv(glGetUniformLocation(LampProgram, "model_lamp"), 1, GL_FALSE, glm::value_ptr(model_lamp));
		glUniformMatrix4fv(glGetUniformLocation(LampProgram, "modelview_lamp"), 1, GL_FALSE, glm::value_ptr(modelview_lamp));
		glUniformMatrix4fv(glGetUniformLocation(LampProgram, "projection_lamp"), 1, GL_FALSE, glm::value_ptr(projection_perspective));
		glDrawArrays(GL_TRIANGLES, 0,mySphere.getNumIndices());

		// ---------------------------------------------------------------------------------------------------------------
		glfwSwapBuffers(window);
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
