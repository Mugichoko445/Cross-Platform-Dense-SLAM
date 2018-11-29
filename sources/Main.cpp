#include <iostream>

#include "App.h"
#include "SLAM/Utilities/IniFileReader.h"
#include "SLAM/Utilities/TUMRGBDUtilities.h"

int main()
{
	glm::mat3 K;
	int width, height;
	std::string wndName, gtPath, cFramePath, dFramePath;

	IniFileReader reader("../../data/input_param.txt");
	reader.readData(K, width, height, wndName, gtPath, cFramePath, dFramePath);

	TUMRGBDUtil::Ptr tum = std::make_shared<TUMRGBDUtil>();
	tum->open(cFramePath, dFramePath, gtPath, K, (width == 320 && height == 240));

	App myApp(width, height, wndName, tum);
	myApp.mainLoop();

	return 0;
}