#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../SOIL2/SOIL2.h"
#include "../include/Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "../include/cube.h"
#include "Sphere.h"

const float arucoSquareDimension = 0.05f;
Sphere mySphere = Sphere(48);

bool loadCameraCalibration(std::string name, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients) {
	std::ifstream inStream(name);
	if (inStream) {
		uint16_t rows;
		uint16_t columns;

		inStream >> rows;
		inStream >> columns;

		cameraMatrix = cv::Mat(cv::Size(columns, rows), CV_64F);

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double read = 0.0f;
				inStream >> read;
				cameraMatrix.at<double>(r, c) = read;
				std::cout << cameraMatrix.at<double>(r, c) << "\n";
			}
		}
		//Distance Coefficients
		inStream >> rows;
		inStream >> columns;

		distanceCoefficients = cv::Mat::zeros(rows, columns, CV_64F);

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double read = 0.0f;
				inStream >> read;
				distanceCoefficients.at<double>(r, c) = read;
				std::cout << distanceCoefficients.at<double>(r, c) << "\n";
			}
		}
		inStream.close();
		return true;
	}
	return false;
}

void drawSquare(
	cv::InputOutputArray image, cv::InputArray cameraMatrix,
	cv::InputArray distCoeffs, cv::InputArray rvec, cv::InputArray tvec,
	float l0
)
{
	float l = l0 * 1.05;  // new square is 5% larger than the aruco marker
	float half_l = l / 2.0;

	// Define the square on the camera frame (this is 3D since the camera
	// frame is 3D).
	std::vector<cv::Point3f> squarePoints;
	squarePoints.push_back(cv::Point3f(half_l, half_l, 0));
	squarePoints.push_back(cv::Point3f(half_l, -half_l, 0));
	squarePoints.push_back(cv::Point3f(-half_l, -half_l, 0));
	squarePoints.push_back(cv::Point3f(-half_l, half_l, 0));

	// Project the square to the image.
	std::vector<cv::Point2f> imagePoints;
	projectPoints(
		squarePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints
	);

	// Draw the square on the image.
	cv::line(image, imagePoints[0], imagePoints[1], cv::Scalar(255, 0, 0), 3);
	cv::line(image, imagePoints[1], imagePoints[2], cv::Scalar(255, 0, 0), 3);
	cv::line(image, imagePoints[2], imagePoints[3], cv::Scalar(255, 0, 0), 3);
	cv::line(image, imagePoints[3], imagePoints[0], cv::Scalar(255, 0, 0), 3);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width = 640, height = 800;
int width2, height2;

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


	GLuint renderingProgram = Utils::createShaderProgram("src/shaders/vertShader.glsl", "src/shaders/fragShader.glsl");
	GLuint bgProgram = Utils::createShaderProgram("src/shaders/bgVertShader.glsl", "src/shaders/bgFragShader.glsl");
/*
	// Camera Matrix & Distortion Parameters
	cv::Mat intrinsic_matrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distortion_parameters;

	//loadCameraCalibration("CAMARA", intrinsic_matrix, distortion_parameters);*/
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

	cube cube1;

	// Video Variables
	cv::Mat frame;
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	cv::aruco::DetectorParameters parameters;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_250);
	cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 5, 0.04, 0.01, markerDictionary);
	//camara por droidcam
	//cv::VideoCapture vid("http://192.168.1.5:4747/video");
	//camara de laptop o pc
	cv::VideoCapture vid(0);
	if (!vid.isOpened()) {
		return -1;
	}

	//std::vector<cv::Vec3d> rotationVectors, translationVectors;
	cv::Mat rotationVectors, translationVectors;

	// ================================================================================
	//CREACION DE LA ESFERA
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	int numIndices = mySphere.getNumIndices();
	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		pvalues.push_back((tex[ind[i]]).s);
		pvalues.push_back((tex[ind[i]]).t);
	}

	// ==================================================================================
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float) , (GLvoid*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	// =================================================================================
	// Texture Stuff
	GLuint texture1, texture2;
	GLuint texture3; //para la luna

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


	////////////////////////////////////////////////////TEXTURA LUNA

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

/////////////////////////////////////////////////////////////////////////////////

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("resources/batman.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// =======================================================================
	// scaling matrix for the 3d object
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	// default values of modelview so that initially when there is no marker, the object is placed behind the camera and it does not show up
	glm::mat4 modelview = glm::mat4(1.0f);
	modelview = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));

	modelview = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));

	// defining the projection matrix
	float near = 0.2f;
	float far = 400.0f;
	//float fx = intrinsic_matrix.at<float>(0, 0);
	//float fy = intrinsic_matrix.at<float>(1, 1);
	//float cx = intrinsic_matrix.at<float>(0, 2);
	//float cy = intrinsic_matrix.at<float>(1, 2);
	/*
	float fx = 612.9770191846128f;
	float fy = 612.9770191846128f;
	float cx = 319.5;
	float cy = 239.5;
	*/

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
			//Draw Markers
			cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
			int valid = estimatePoseBoard(markerCorners, markerIds, board, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors);
			//frame = cube1.drawcube(frame, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors);
			//drawSquare(frame, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors, 0.05f);
			// if at least one board marker detected
			if (valid > 0) {
				//frame = cube1.drawcube(frame, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors);
				cv::Rodrigues(rotationVectors, rot_mat);

				modelview = { rot_mat.at<double>(0,0),  rot_mat.at<double>(0,1),  rot_mat.at<double>(0,2),  translationVectors.at<double>(0),
							 -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -translationVectors.at<double>(1),
							 -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -translationVectors.at<double>(2),
									   0.0f,                      0.0f,                     0.0f,                   1.0f };
				modelview = glm::transpose(modelview);
				cv::aruco::drawAxis(frame, intrinsic_matrix, distortion_parameters, rotationVectors, translationVectors, 0.05f);
			}
		}

		//modelview = model * modelview;
		modelview = glm::scale(modelview, glm::vec3(0.07f, 0.07f, 0.07f));
		modelview = glm::translate(modelview, glm::vec3(1.4f, 1.4f, 1.0f));
		//modelview = glm::scale(modelview, glm::vec3(0.01f, 0.01f, 0.01f));


		//cv::imshow("Draw Axis", frame);
		//if (cv::waitKey(10) == 27) break;  // if close is pressed on opencv window, exit the program

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(renderingProgram, "batman_texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(renderingProgram, "wood_texture"), 1);

		glUseProgram(renderingProgram);
		//std::cout << glm::to_string(projection_perspective) << std::endl;

		//glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
		glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
		glBindVertexArray(0);

		// draw bg ---------------------------------------------------------------------------------------------
		glUseProgram(bgProgram);
		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bgProgram, "webcam_texture"), 2);

		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
		glBindVertexArray(0);
		// -----------------------------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
