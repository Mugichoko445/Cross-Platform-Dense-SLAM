#include "App.h"

App::App(
	int width,
	int height,
	const std::string &wndName,
	const TUMRGBDUtil::Ptr tum
) : Window(width, height, wndName), tum(tum)
{
	glfwSetWindowSize(window, int(width * 4.0f / 3.0f), height);

	TUMFrame dsFrame;
	this->tum->readRGBDFrame(dsFrame);

	std::map<std::string, const gl::Shader::Ptr> progs4SLAM;
	std::string pathToSLAMShaders("../../shaders/SLAM/");
	slam.loadShaders(progs4SLAM, pathToSLAMShaders);

	for (auto &f : frame)
	{
		f.create(dsFrame.frame_c.cols, dsFrame.frame_c.rows, rgbd::ICPConstParam::MAX_LEVEL,
			rgbd::ICPConstParam::MIN_DEPTH, rgbd::ICPConstParam::MAX_DEPTH, this->tum->getIntrinsicParam(), progs4SLAM);
		f.update(dsFrame.frame_c.data, dsFrame.frame_d.data);
	}

	slam.init(frame[rgbd::FRAME::CURRENT], frame[rgbd::FRAME::VIRTUAL], this->tum->getIntrinsicParam(), progs4SLAM);

	std::string pathToAppShaders("../../shaders/");
	progs.insert(std::make_pair("ScreenQuad", std::make_shared<gl::Shader>(pathToAppShaders + "ScreenQuad.vert", pathToAppShaders + "ScreenQuad.frag")));
	progs["ScreenQuad"]->setUniform("isYFlip", 1);
	progs["ScreenQuad"]->setUniform("maxDepth", rgbd::ICPConstParam::MAX_DEPTH);
}

App::~App()
{
}

bool App::runSLAM()
{
	glViewport(0, 0, width, height);

	bool status = true;
	TUMFrame dsFrame;
	if (tum->readRGBDFrame(dsFrame))
	{
		clock_t start_update_frame1 = clock();
		frame[rgbd::FRAME::CURRENT].update(dsFrame.frame_c.data, dsFrame.frame_d.data);
		std::cout << "  Update frame #1: " << (clock() - start_update_frame1) / (double)CLOCKS_PER_SEC << " sec" << std::endl;

		glm::mat4 T = slam.calcDevicePose(frame[rgbd::FRAME::CURRENT], frame[rgbd::FRAME::VIRTUAL]);
		slam.updateGlobalMap(frame[rgbd::FRAME::CURRENT], frame[rgbd::FRAME::VIRTUAL]);
	}
	else status = false;

	return status;
}

void App::drawAssets()
{
	glDisable(GL_DEPTH_TEST);
	int subWidth = int(width / 3.0f);
	int subHeight = int(height / 3.0f);

	glViewport(0, 0, width, height);
	progs["ScreenQuad"]->use();
	progs["ScreenQuad"]->setUniform("mapType", int(rgbd::MAP_TYPE::COLOR_BGR));
	quad.render(frame[rgbd::FRAME::CURRENT].getColorMap());
	progs["ScreenQuad"]->disuse();

	glViewport(width, 0, subWidth, subHeight);
	progs["ScreenQuad"]->use();
	progs["ScreenQuad"]->setUniform("mapType", int(rgbd::MAP_TYPE::NORMAL));
	quad.render(frame[rgbd::FRAME::VIRTUAL].getNormalMap());
	progs["ScreenQuad"]->disuse();
	
	glViewport(width, subHeight, subWidth, subHeight);
	progs["ScreenQuad"]->use();
	progs["ScreenQuad"]->setUniform("mapType", int(rgbd::MAP_TYPE::COLOR_RGB));
	quad.render(frame[rgbd::FRAME::VIRTUAL].getColorMap());
	progs["ScreenQuad"]->disuse();
	
	glViewport(width, 2 * subHeight, subWidth, subHeight);
	progs["ScreenQuad"]->use();
	progs["ScreenQuad"]->setUniform("mapType", int(rgbd::MAP_TYPE::DEPTH));
	quad.render(frame[rgbd::FRAME::CURRENT].getDepthMap());
	progs["ScreenQuad"]->disuse();

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
}

void App::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		clearBuffers(0.1f, 0.7f, 0.3f, 0.0f);

		if (!runSLAM()) break;
		drawAssets();

		swapBuffers();
	}

	cv::destroyAllWindows();
}