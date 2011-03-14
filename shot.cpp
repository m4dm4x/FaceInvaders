#include "shot.h"

Shot::Shot(const cv::Point &position, const int &direction, const cv::Scalar &color, cv::Mat &image):
	position(position), direction(direction), color(color), image(image), out(false){
	/* Empty Constructor */
}

Shot::~Shot(void) { /* Nothing to do */ }

void Shot::update(void){
	position.y += direction;
	if (image.size().height <= (position.y + Shot::HEIGHT )) out = true;
	if (position.y <= 0) out = true;
}

void Shot::update(const cv::Point &newpos) {
	position = newpos;
	out = false;
	update();
}

void Shot::draw(void){
	if (this->out) return;
	cv::line(image, position, cv::Point(position.x, position.y + Shot::HEIGHT), this->color, 5, 1);
}


void Shot::setOut(void){
	this->out = true;
}

bool Shot::isOut(void){
	return this->out;
}

cv::Rect Shot::getRectangle(void){
	if (this->out)
		return cv::Rect(0, 0, 0, 0);
	else
		return cv::Rect(position.x, position.y, 1, Shot::HEIGHT);
}
