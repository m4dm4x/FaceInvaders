#include "player.h"

#include <opencv/highgui.h>
#include "gameimage.h"

inline static void drawCross(cv::Mat &img, const cv::Point &pos, int yShot) {
	//cv::line(img, cv::Point(pos.x - 10, pos.y), cv::Point(pos.x + 10, pos.y), cv::Scalar(255,255,255),2,1);
	//cv::line(img, cv::Point(pos.x, pos.y - 10), cv::Point(pos.x, pos.y + 10), cv::Scalar(255,255,255),2,1);
	//cv::circle(img, pos, 10, cv::Scalar(255,255,0),5);
	const cv::Size isize( img.size() );
	cv::line(img, cv::Point(0, pos.y), cv::Point(isize.width, pos.y), cv::Scalar::all(255),1);
	cv::line(img, cv::Point(pos.x, 0), cv::Point(pos.x, yShot), cv::Scalar(0,127,255), 3);
	cv::line(img, cv::Point(pos.x, yShot), cv::Point(pos.x, isize.height), cv::Scalar::all(255),2);
}

Player::Player(const GameImage& playerImage, const cv::Size& camSize, int yShot):
	playerImage(playerImage),
	camSize(camSize),
	yShot(yShot),
	pos(camSize.width / 2, camSize.height / 2)
{

}

Player::~Player() { }

inline static int irange(int min_, int i, int max_) {
	return (i < min_) ? min_ : ((i > max_) ? max_ : i);
}

void Player::onPaint(cv::Mat& image) const {
	drawCross(image, pos, yShot);
	const int xPlayer = pos.x - (playerImage.size().width / 2);
	const int yPlayer = camSize.height - playerImage.size().height;
	cv::Point topLeft(irange(0, xPlayer, camSize.width), yPlayer); 
	playerImage.paint(image, topLeft);
}

void Player::onUpdate() { }

bool Player::isShooting() const {
	return (pos.y < yShot);
}

cv::Point Player::facePosition() const {
	return pos;
}

void Player::setFacePosition(const cv::Point& newpos) {
	pos = newpos;
}

cv::Rect Player::getRect() const {
	const int xPlayer = pos.x - (playerImage.size().width / 2);
	const int yPlayer = camSize.height - playerImage.size().height;
	cv::Point topLeft(irange(0, xPlayer, camSize.width), yPlayer); 
	return cv::Rect(topLeft, playerImage.size());
}
