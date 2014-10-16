#ifndef PLAYERPOSITION_H
#define PLAYERPOSITION_H

#include <vector>
#include <string>

#include <opencv/cv.hpp>

//#define PLAYER_POSITION_DEBUG

class Player;

class PlayerPosition {
private:
	Player &player;
	cv::CascadeClassifier cascade;
	std::vector<cv::Rect> faces;
	cv::Mat cam;
	cv::Mat cgray;
public:
	PlayerPosition(Player &player, const std::string &haarcascadeFile, const cv::Size &size);
	bool update(const cv::Mat &img);
};

#endif // PLAYERPOSITION_H
