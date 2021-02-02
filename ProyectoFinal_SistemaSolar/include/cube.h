#ifndef CUBE_H
#define CUBE_H

#include <opencv2/opencv.hpp>
#include <vector>

class cube
{
private:
	std::vector< cv::Point3f > cube_obj_pts;
	std::vector< cv::Point2f > cube_img_pts;

public:
	cube();

	//cv::Mat drawcube(cv::Mat, cv::Mat, cv::Mat, cv::Vec3d, cv::Vec3d);
	cv::Mat drawcube(cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat);
};

#endif