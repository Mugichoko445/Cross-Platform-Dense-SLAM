#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>

class IniFileReader
{
private:
	std::ifstream ifs;

public:
	IniFileReader(
		const std::string &fileName
	);
	~IniFileReader();

	void readData(
		glm::mat3 &K,
		int &width,
		int &height,
		std::string &windowName,
		std::string &groundTruthPath,
		std::string &colorFramePath,
		std::string &depthFramePath
	);
};