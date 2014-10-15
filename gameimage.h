#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H

#include <opencv/cxcore.hpp>

class GameImage {
	cv::Mat mask;
	cv::Mat image;
	//int xHalf;
	//int yHalf;
public:
	GameImage(const std::string &filename);
	cv::Size size() const;
	void paint(cv::Mat &img, const cv::Point &topLeft) const;
};

#endif // GAMEIMAGE_H
