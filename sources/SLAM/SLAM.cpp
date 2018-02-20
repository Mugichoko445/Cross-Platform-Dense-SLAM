#include "SLAM.h"

namespace rgbd
{
	SLAM::SLAM()
	{
		vT.push_back(glm::mat4());
	}

	SLAM::~SLAM()
	{
	}

	void SLAM::loadShaders(
		std::map<std::string, const gl::Shader::Ptr> &progs,
		const std::string &folderPath
	)
	{
		progs.insert(std::make_pair("BilateralFilter", std::make_shared<gl::Shader>(folderPath + "BilateralFilter.comp")));
		progs.insert(std::make_pair("CalcVertexMap", std::make_shared<gl::Shader>(folderPath + "CalcVertexMap.comp")));
		progs.insert(std::make_pair("CalcNormalMap", std::make_shared<gl::Shader>(folderPath + "CalcNormalMap.comp")));
		progs.insert(std::make_pair("VirtualMapGeneration", std::make_shared<gl::Shader>(folderPath + "VirtualMapGeneration.vert", folderPath + "VirtualMapGeneration.frag")));
		progs.insert(std::make_pair("ProjectiveDataAssoc", std::make_shared<gl::Shader>(folderPath + "ProjectiveDataAssoc.comp")));
		progs.insert(std::make_pair("MultiplyMatrices", std::make_shared<gl::Shader>(folderPath + "MultiplyMatrices.comp")));
		progs.insert(std::make_pair("DownSamplingC", std::make_shared<gl::Shader>(folderPath + "DownSamplingC.comp")));
		progs.insert(std::make_pair("DownSamplingD", std::make_shared<gl::Shader>(folderPath + "DownSamplingD.comp")));
		progs.insert(std::make_pair("DownSamplingV", std::make_shared<gl::Shader>(folderPath + "DownSamplingV.comp")));
		progs.insert(std::make_pair("DownSamplingN", std::make_shared<gl::Shader>(folderPath + "DownSamplingN.comp")));
		progs.insert(std::make_pair("IndexMapGeneration", std::make_shared<gl::Shader>(folderPath + "IndexMapGeneration.vert", folderPath + "IndexMapGeneration.frag")));
		progs.insert(std::make_pair("GlobalMapUpdate", std::make_shared<gl::Shader>(folderPath + "GlobalMapUpdate.comp")));
		progs.insert(std::make_pair("SurfaceSplatting", std::make_shared<gl::Shader>(folderPath + "SurfaceSplatting.vert", folderPath + "SurfaceSplatting.frag", folderPath + "SurfaceSplatting.geom")));
		progs.insert(std::make_pair("UnnecessaryPointRemoval", std::make_shared<gl::Shader>(folderPath + "UnnecessaryPointRemoval.comp")));
	}

	void SLAM::init(
		const rgbd::Frame &currentFrame,
		const rgbd::Frame &virtualFrame,
		const glm::mat3 &K,
		const std::map<std::string, const gl::Shader::Ptr> &progs
	)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		
		int width = currentFrame.getWidth();
		int height = currentFrame.getHeight();

		icp = std::make_shared<rgbd::PyramidricalICP>(width, height, K, progs);
		gMap = std::make_shared<rgbd::GlobalMap>(width, height, K, progs);

		std::cout << "map initialization..." << std::endl;
		for (int idx = 0; idx < 1; ++idx)
		{
			std::cout << "Map size: " << gMap->getMapSize() << std::endl;
			gMap->genVirtualFrame(virtualFrame, glm::mat4());
			virtualFrame.update();

			gMap->updateGlobalMap(currentFrame, glm::mat4(), 0);
			gMap->removeUnnecessaryPoints(0);
			gMap->genIndexMap(glm::mat4());
		}
		std::cout << "done!" << std::endl;
	}

	glm::mat4 SLAM::calcDevicePose(
		const rgbd::Frame &currentFrame,
		const rgbd::Frame &virtualFrame
	)
	{
		clock_t start_icp = clock();
		static glm::mat4 T;
		icp->calc(virtualFrame, currentFrame, T);
		vT.push_back(vT.back() * T);
		std::cout << "  ICP: " << (clock() - start_icp) / (double)CLOCKS_PER_SEC << " sec" << std::endl;

		return vT.back();
	}

	void SLAM::updateGlobalMap(
		const rgbd::Frame &currentFrame,
		const rgbd::Frame &virtualFrame
	)
	{
		glm::mat4 invT = glm::inverse(vT.back());

		clock_t start_idx_map = clock();
		gMap->genIndexMap(invT);
		std::cout << "  Index map: " << (clock() - start_idx_map) / (double)CLOCKS_PER_SEC << " sec" << std::endl;

		clock_t start_update_map = clock();
		gMap->updateGlobalMap(currentFrame, vT.back(), static_cast<int>(vT.size()));
		std::cout << "  Update map: " << (clock() - start_update_map) / (double)CLOCKS_PER_SEC << " sec" << std::endl;
		std::cout << "  --> Map size: " << gMap->getMapSize() << std::endl;

		clock_t start_remove_pts = clock();
		gMap->removeUnnecessaryPoints(static_cast<int>(vT.size()));
		std::cout << "  Remove points: " << (clock() - start_remove_pts) / (double)CLOCKS_PER_SEC << " sec" << std::endl;
		std::cout << "  --> Removed map size: " << gMap->getMapSize() << std::endl;

		clock_t start_virtual_frame = clock();
		gMap->genVirtualFrame(virtualFrame, invT);
		std::cout << "  Virtual frame: " << (clock() - start_virtual_frame) / (double)CLOCKS_PER_SEC << " sec" << std::endl;

		clock_t start_update_frame2 = clock();
		virtualFrame.update();
		std::cout << "  Update frame #2: " << (clock() - start_update_frame2) / (double)CLOCKS_PER_SEC << " sec" << std::endl;
	}
}