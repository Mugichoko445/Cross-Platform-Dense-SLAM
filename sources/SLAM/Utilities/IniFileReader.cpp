#include "IniFileReader.h"

IniFileReader::IniFileReader(
	const std::string &fileName
)
{
	ifs.open(fileName);
}

IniFileReader::~IniFileReader() { }

void IniFileReader::readData(
	glm::mat3 &K,
	int &width,
	int &height,
	std::string &windowName,
	std::string &groundTruthPath,
	std::string &colorFramePath,
	std::string &depthFramePath
)
{
	std::string line;
	std::istringstream iss;

	std::getline(ifs, line);

	// Read intrinsic parameters
	std::getline(ifs, line);
	std::getline(ifs, line);
	iss = std::istringstream(line);
	K = glm::mat3(1.0f);
	iss >> K[0][0] >> K[1][1] >> K[2][0] >> K[2][1];

	// Read image size
	std::getline(ifs, line);
	std::getline(ifs, line);
	iss = std::istringstream(line);
	iss >> width >> height;

	// Read window name
	std::getline(ifs, line);
	std::getline(ifs, windowName);

	// Read file names
	std::getline(ifs, line);
	std::getline(ifs, groundTruthPath);
	std::getline(ifs, line);
	std::getline(ifs, colorFramePath);
	std::getline(ifs, line);
	std::getline(ifs, depthFramePath);
}