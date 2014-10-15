#include "gameimage.h"

#include <opencv/highgui.h>

GameImage::GameImage(const std::string& filename) {
	image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
	mask = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	//xHalf = image.size().width / 2;
	//yHalf = image.size().height / 2;
}

cv::Size GameImage::size() const {
	return image.size();
}

void GameImage::paint(cv::Mat& img, const cv::Point& topLeft) const {
	const cv::Rect roi(topLeft, image.size());
	cv::Mat image_roi = img(roi);
	image_roi.setTo(0, mask);
	image_roi += image;
}
