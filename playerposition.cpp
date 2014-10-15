#include "playerposition.h"

#ifdef PLAYER_POSITION_DEBUG
#include <opencv/highgui.h>
#endif

#include "player.h"

PlayerPosition::PlayerPosition(Player &player, const std::string &haarcascadeFile, const cv::Size& size) :
	player(player), cam(cv::Size(size.width / 2, size.height / 2),CV_8UC1), cgray(cam.size(),CV_8UC1)
{
	if ( !cascade.load( haarcascadeFile ) ) {
		throw std::runtime_error("Error loading haarcascades!");
	}
#ifdef PLAYER_POSITION_DEBUG
	cv::namedWindow("SpaceInvaders-Player", CV_WINDOW_AUTOSIZE);
#endif
}

bool PlayerPosition::update(const cv::Mat& img) {
	cv::resize(img, cam, cam.size());
	cv::cvtColor(cam, cgray, CV_BGR2GRAY);
	cv::equalizeHist(cgray, cgray);
	faces.clear();
	cascade.detectMultiScale(cgray, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
#ifdef PLAYER_POSITION_DEBUG
	if (!faces.empty()) {
		cv::rectangle(cgray,faces[0],cv::Scalar(255,255,255));
	}
	cv::imshow("SpaceInvaders-Player", cgray);
#endif
	
	if (!faces.empty()) {
		const cv::Rect &face = faces[0];
		const cv::Point facePoint(2 * (face.x + (face.width / 2)), 2 * (face.y + (face.height / 2)));
		player.setFacePosition(facePoint);
	}
	
	return !faces.empty();
}