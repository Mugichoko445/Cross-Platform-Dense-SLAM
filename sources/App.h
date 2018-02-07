#pragma once

#include <iostream>
#include <map>

#include <opencv2/opencv.hpp>

#include "GLCore/Shader.h"
#include "GLCore/Window.h"
#include "GLExt/Quad.h"

#include "SLAM/SLAM.h"
#include "SLAM/Frame/Frame.h"
#include "SLAM/Utilities/IniFileReader.h"
#include "SLAM/Utilities/TUMRGBDUtilities.h"

class App : gl::Window
{
private:
	TUMRGBDUtil::Ptr tum;
	std::array<rgbd::Frame, 2> frame;
	rgbd::SLAM slam;

	gl::Quad quad;
	std::map<std::string, const gl::Shader::Ptr> progs;

public:
	App(
		int width,
		int height,
		const std::string &wndName,
		const TUMRGBDUtil::Ptr tum
	);
	~App();

	void mainLoop();

	bool runSLAM();
	void drawAssets();
};