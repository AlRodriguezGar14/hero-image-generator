#include <string>
#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

cv::Mat resizeAndCrop(const cv::Mat& image, int targetWidth, int targetHeight) {
	std::cout << "Resizing the image..." << std::endl;
    int originalWidth = image.cols;
    int originalHeight = image.rows;
    float targetAspect = static_cast<float>(targetWidth) / targetHeight;
    float originalAspect = static_cast<float>(originalWidth) / originalHeight;

    int newWidth, newHeight;
    if (originalAspect > targetAspect) {
        newWidth = static_cast<int>(targetHeight * originalAspect);
        newHeight = targetHeight;
    } else {
        newWidth = targetWidth;
        newHeight = static_cast<int>(targetWidth / originalAspect);
    }

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);

    int xCrop = (newWidth - targetWidth) / 2;
    int yCrop = (newHeight - targetHeight) / 2;

	std::cout << "Cropping the image..." << std::endl;
    cv::Rect cropRegion(xCrop, yCrop, targetWidth, targetHeight);
    return resizedImage(cropRegion);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}
	std::string movieName;
	std::cout << "Movie name: ";
	std::getline(std::cin, movieName);
	size_t spacePos = movieName.find(" ");
	while (spacePos != std::string::npos) {
		movieName.replace(spacePos, 1, "_");
		spacePos = movieName.find(" ");
	}
	std::cout << "Fetching the video..." << std::endl;
	std::string videopath = argv[1];

	cv::VideoCapture cap(videopath);
	if (!cap.isOpened()) {
		std::cerr << "Error: Could not open video file " << videopath << std::endl;
		return 1;
	}
	std::string timecodeStr;
	std::cout << "Timecode: ";
	std::getline(std::cin, timecodeStr);
	int timecode = std::stoi(timecodeStr);
	double fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "Setting the timecode to " << timecode << " seconds" << std::endl;
	cap.set(cv::CAP_PROP_POS_FRAMES, timecode * fps);
	cv::Mat screenshot;
	cap.read(screenshot);
	if (screenshot.empty()) {
		std::cout << "Invalid timecode" << std::endl;
		return 1;
	}
	cv::imwrite(movieName + "_still_16x9_fhfix.jpeg", resizeAndCrop(screenshot, 1920, 1080));
	std::cout << "Screenshot generated" << std::endl;
	return 0;
}