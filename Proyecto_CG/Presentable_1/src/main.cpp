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
 * DICT_5X5_250=6
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
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(1, 2, 0.04, 0.01, dictionary);
	cv::Mat boardImage;
	board->ids={10,10};
	board->draw( cv::Size(600, 500), boardImage, 10, 1 );
	cv::imwrite("marker26.png", boardImage);


    return 0;
}
