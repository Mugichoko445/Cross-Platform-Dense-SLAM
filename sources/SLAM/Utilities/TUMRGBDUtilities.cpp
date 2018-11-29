#include "TUMRGBDUtilities.h"

const float TUMConstParam::DFACTOR = 5000.0f;

TUMRGBDUtil::TUMRGBDUtil()
{
}

TUMRGBDUtil::~TUMRGBDUtil()
{
}

void TUMRGBDUtil::open(
	const std::string &cFramePath,	// Path to "rgb.txt"
	const std::string &dFramePath,	// Path to "depth.txt"
	const std::string &gtPath,		// Path to "groundtruth.txt"
	const glm::mat3 &K,
	bool isQVGA
)
{
	this->K = K;
	this->isQVGA = isQVGA;

	path_c = std::experimental::filesystem::path(cFramePath).parent_path().string();
	path_d = std::experimental::filesystem::path(dFramePath).parent_path().string();

	ifs_c.open(cFramePath);
	if (!ifs_c.is_open()) std::cout << "Failed to open: " << cFramePath << std::endl;
	ifs_d.open(dFramePath);
	if (!ifs_d.is_open()) std::cout << "Failed to open: " << dFramePath << std::endl;
	ifs_gt.open(gtPath);
	if (!ifs_gt.is_open()) std::cout << "Failed to open: " << gtPath << std::endl;

	// Ignore first three lines
	std::string line;
	std::getline(ifs_c, line); std::getline(ifs_c, line); std::getline(ifs_c, line);
	std::getline(ifs_d, line); std::getline(ifs_d, line); std::getline(ifs_d, line);
	std::getline(ifs_gt, line); std::getline(ifs_gt, line); std::getline(ifs_gt, line);
}

bool TUMRGBDUtil::readRGBDFrame(
	TUMFrame &frame
)
{
	std::string line_c, line_d;
	std::string name_c, name_d;

	if (std::getline(ifs_c, line_c) && std::getline(ifs_d, line_d))
	{
		// color frame
		std::istringstream iss_c(line_c);
		iss_c >> frame.ts_c >> name_c;
		frame.frame_c = cv::imread(path_c + "/" + name_c);
		cv::cvtColor(frame.frame_c, frame.frame_c, CV_BGR2BGRA);
		if (isQVGA) cv::resize(frame.frame_c, frame.frame_c, cv::Size(320, 240));
		// depth frame
		std::istringstream iss_d(line_d);
		iss_d >> frame.ts_d >> name_d;
		frame.frame_d = cv::imread(path_d + "/" + name_d, -1);
		if (isQVGA) cv::resize(frame.frame_d, frame.frame_d, cv::Size(320, 240));
		frame.frame_d.convertTo(frame.frame_d, CV_32F);
		frame.frame_d /= TUMConstParam::DFACTOR;

		std::cout << "Read: " << name_c << " and " << name_d << std::endl;

		return true;
	}
	else
	{
		std::cerr << "Failed to read frames." << std::endl;
		return false;
	}
}

bool TUMRGBDUtil::readGtData(
	std::string &timestamp,
	glm::vec3 &pos,
	glm::quat &qt
)
{
	std::string line;
	if (std::getline(ifs_gt, line))
	{
		std::istringstream iss(line);
		iss >> timestamp >> pos.x >> pos.y >> pos.z >> qt.x >> qt.y >> qt.z >> qt.w;
		return true;
	}
	else
	{
		return false;
	}
}

bool TUMRGBDUtil::readGtData(
	std::string &timestamp,
	glm::mat4 &T
)
{
	glm::vec3 t;
	glm::quat qt;
	if (readGtData(timestamp, t, qt))
	{
		glm::mat3 R = glm::mat3_cast(qt);
		glm::vec3 t(t.x, t.y, t.z);
		T = glm::lookAt(t, t + R[2], -R[1]);

		return true;
	}
	else
	{
		return false;
	}
}

glm::mat3 TUMRGBDUtil::getIntrinsicParam()
{
	return K;
}