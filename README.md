A cross-platform implementation of RGB-D SLAM proposed by Keller et al.
* C++, OpenGL, and OpenCV only
* **No** CUDA and **No** OpenCL

[M. Keller, D. Lefloch, M. Lambers, S. Izadi, T. Weyrich, and A. Kolb: "Real-Time 3D Reconstruction in Dynamic Scenes Using Point-Based Fusion," Proc. Int. Conf. on 3D Vision, pp. 1 - 8, 2013.](http://ieeexplore.ieee.org/document/6599048/)

# Requirements

* C++ compiler
* OpenGL 4.3 or later
    * requires [GLEW](http://glew.sourceforge.net/), [GLFW3](http://www.glfw.org/), and [GLM](https://glm.g-truc.net/0.9.8/index.html)
* [OpenCV 2.X or later](https://opencv.org/)
    * used as image read/write and a linear problem solver (QR decomposition)
    * thus, you can easily replace OpenCV with other libraries
* Lisence
	* Lisence free but limited to **research purpose only**
	* Note that the code is **NOT** the original implementation (i.e., results may be different from the ones in the original paper)
	* BUT I'm pleased to have any kinds of feedback from you!!

# Usage

1. Prepare your dataset
	* e.g., [TUM RGB-D dataset](https://vision.in.tum.de/data/datasets/rgbd-dataset)
	* Read/write interfaces for TUM RGB-D dataset is implemented in the code
	* If you need to record your own image sequences, then [this implementation](https://github.com/chaowang15/RGBDCapture) would be helpful
2. Open "data/input_param.txt" to change parameters
3. Build the program
4. Run it

# Example Result

[![YouTube Video of Cross-Platform RGB-D SLAM](https://img.youtube.com/vi/0sRPtVyJc5w/0.jpg)](https://www.youtube.com/watch?v=0sRPtVyJc5w)

# Tested Environment List
<table>
	<tbody>
		<tr>
			<td colspan="2"><b>Hardware</b></td>
			<td colspan="5"><b>Software</b></td>
		</tr>
		<tr>
			<td><b>CPU</b></td>
			<td><b>GPU</b></td>
			<td><b>OS</b></td>
			<td><b>GLEW</b></td>
			<td><b>GLFW</b></td>
			<td><b>GLM</b></td>
			<td><b>OpenCV</b></td>
		</tr>
		<tr>
			<td>Intel Core i7 6567U</td>
			<td>Intel Iris Graphics 550</td>
			<td>Win 10</td>
			<td>1.13.0</td>
			<td>3.2</td>
			<td>0.9.8.5</td>
			<td>3.3.1</td>
		</tr>
		<tr>
			<td>Intel Core i7 4770S</td>
			<td>NVIDIA GeForce GTX760</td>
			<td>Win 10</td>
			<td>2.1.0</td>
			<td>3.2.1</td>
			<td>0.9.8.5</td>
			<td>3.3.0</td>
		</tr>
		<tr>
			<td colspan="2"><a href="https://developer.nvidia.com/embedded/buy/jetson-tx2" target="_blank">NVIDIA Jetson TX2</a></td>
			<td>Win 10</td>
			<td>1.13.02.1.0</td>
			<td>3.2.1</td>
			<td>0.9.7.2</td>
			<td>2.4.13.1</td>
		</tr>
	</tbody>
</table>

# Current issues

* Slow (<= 10 fps)
* No dynamic object detection

# Progress Reports (in Japanese)

[http://mugichoko.hatenablog.com/archive/category/SLAM](http://mugichoko.hatenablog.com/archive/category/SLAM)