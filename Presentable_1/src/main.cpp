/*****************************************************************************
 * Autores : Joaqu�n S�nchez, Juan Diego C�rdova, Rosinaldo Rivera
 * Fecha : 30/12/2020
 * Curso : Computaci�n Gr�fica
 * Profesor : Herminio Paucar
 *****************************************************************************/

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

// opencv headers
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

// basic headers
#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;


/************************* dictionary:
 * DICT_4X4_50=0
 * DICT_4X4_100=1
 * DICT_4X4_250=2
 * DICT_4X4_1000=3
 * DICT_5X5_50=4
 * DICT_5X5_100=5
 * DICT_5X5_250=6 este es el que vamos a trabajar
 * DICT_5X5_1000=7
 * DICT_6X6_50=8
 * DICT_6X6_100=9
 * DICT_6X6_250=10
 * DICT_6X6_1000=11
 * DICT_7X7_50=12
 * DICT_7X7_100=13
 * DICT_7X7_250=14
 * DICT_7X7_1000=15
 * DICT_ARUCO_ORIGINAL = 16
 *************************/

int main(int argc, char **argv)
{
	cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

	int dict_number=6;
	vector<int> ArrayIDdelSol;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(dict_number));
	cv::Ptr<cv::aruco::GridBoard> boardSol = cv::aruco::GridBoard::create(5, 5, 0.04, 0.01, dictionary);
	cv::Mat boardImage;
	for(int j=0;j<25;j++){
		ArrayIDdelSol.push_back(j); //Asigno los ids para un arreglo
	}
	for(int i=0;i<25;i++){
		boardSol->ids[i]=ArrayIDdelSol.at(i); //para cada posicion del board asigno los valores del vector
	}
	boardSol->draw( cv::Size(600,500), boardImage, 10, 1 );
	cv::imwrite("MarcadordelSol.png", boardImage);

	//ENCENDIDO DE LA CAMARA
	int wait_time = 10;
	int id_camera = 0; // 0 = camera default, 1 = second camera
	// IP DE CAMARA DE CELULAR CON LA APP DROIDCAM
	const std::string videoStreamAddress="http://192.168.1.5:4747/video";
	cv::VideoCapture in_video;
	double fx, fy, cx, cy, k1, k2, k3, p1, p2;
	fx = 955.8925;
	fy = 955.4439;
	cx = 296.9006;
	cy = 215.9074;
	k1 = -0.1523;
	k2 = 0.7722;
	k3 = 0;
	p1 = 0;
	p2 = 0;
	cv::Mat cameraMatrix = (cv::Mat_<float>(3, 3) <<
		fx, 0.0, cx,
		0.0, fy, cy,
		0.0, 0.0, 1.0);
	cv::Mat distCoeffs = (cv::Mat_<float>(5, 1) << k1, k2, p1, p2, k3);
	in_video.open(id_camera); //CAMARA DE LA LAPTOP CONECTADA POR USB
	//in_video.open(videoStreamAddress); // CAMARA DEL CELULAR CON DIRECCION IP
	if (!in_video.isOpened()) {
		std::cerr << "failed to open camera (id=" << id_camera<<")." << std::endl;
		return 1;
	}
	while (in_video.grab()) {
		cv::Mat image, image_copy;
		in_video.retrieve(image);
		image.copyTo(image_copy);
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f>> corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);

		// If at least one marker detected
		if (ids.size() > 0){
			cv::aruco::drawDetectedMarkers(image_copy, corners, ids);
			cv::Vec3d rvec, tvec;
			int valid =cv::aruco::estimatePoseBoard(corners, ids, boardSol, cameraMatrix, distCoeffs, rvec, tvec, false);
			if(valid > 0)
				cv::aruco::drawAxis(image_copy, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
		}
		imshow("Aruco Marker Detected", image_copy);
		char key = (char)cv::waitKey(wait_time);
		if (key == 27) // ascii 27 = ESC
			break;
	}

	in_video.release();


    return 0;
}