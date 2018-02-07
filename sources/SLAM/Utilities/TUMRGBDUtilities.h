#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/detail/setup.hpp>
#include <glm/gtc/quaternion.hpp>

// See: http://vision.in.tum.de/data/datasets/rgbd-dataset/file_formats#intrinsic_camera_calibration_of_the_kinect
struct TUMConstParam
{
	static const float DFACTOR;
};

struct TUMFrame
{
	double ts_c, ts_d;			// time stamps for color and depth frames
	cv::Mat frame_c, frame_d;	// color and depth frames
};

class TUMRGBDUtil
{
private:
	std::ifstream ifs_c;	// for color frames
	std::ifstream ifs_d;	// for depth frames
	std::ifstream ifs_gt;	// for ground truth
	std::string path_c;		// path without file name
	std::string path_d;		// path without file name
	glm::mat3 K;			// intrinsic parameters
	bool isQVGA;

public:
	TUMRGBDUtil();
	~TUMRGBDUtil();

	void open(
		const std::string &cFramePath,	// Path to "rgb.txt"
		const std::string &dFramePath,	// Path to "depth.txt"
		const std::string &gtPath,		// Path to "groundtruth.txt"
		const glm::mat3 &K,
		bool isQVGA = false
	);

	bool readRGBDFrame(
		TUMFrame &frame
	);

	bool readGtData(
		std::string &timestamp,
		glm::vec3 &pos,		// Position
		glm::quat &qt		// Quaternion
	);
	bool readGtData(
		std::string &timestamp,
		glm::mat4 &T
	);

	glm::mat3 getIntrinsicParam();

	typedef std::shared_ptr<TUMRGBDUtil> Ptr;
};