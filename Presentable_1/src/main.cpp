/*****************************************************************************
 * Autores : Joaquín Sánchez, Juan Diego Córdova, Rosinaldo Rivera
 * Fecha : 30/12/2020
 * Curso : Computación Gráfica
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


    return 0;
}
