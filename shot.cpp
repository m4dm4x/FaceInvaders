#include "shot.h"

static const int SHOT_HEIGHT = 20;

Shot::Shot():
	position(cv::Point(0,0)),
	speed(0),
	color(cv::Scalar::all(0)),
	height(-SHOT_HEIGHT)
{
}

Shot::Shot(const cv::Point& position, int speed, const cv::Scalar& color, const cv::Size& size):
	position(position),
	speed(speed),
	color(color),
	height(size.height - SHOT_HEIGHT) // precompute height so we dont have to do it over and over in isValid
{

}

Shot::~Shot() {
	
}

bool Shot::isValid() const {
	return (position.y > 0) && (position.y < height);
}

void Shot::onUpdate() {
	if (isValid()) {
		position.y += speed;
	}
}

void Shot::onPaint(cv::Mat& image) const {
	if (isValid()) {
		cv::line(image, position, cv::Point(position.x, position.y + SHOT_HEIGHT), color, 5, 1);
	}
}

cv::Rect Shot::getRect() const {
	return cv::Rect(position.x - 1, position.y, 3, SHOT_HEIGHT);
}
