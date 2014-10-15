#include "invader.h"
#include <opencv/highgui.h>
#include <cmath>
#include "gameimage.h"

Invader::Invader(const GameImage &invaderImage, const cv::Size &camSize, const cv::Point &position, int speed) :
	invaderImage(invaderImage),
	camSize(camSize),
	pos(position),
	mov(speed, speed)
{
}

Invader::~Invader(){ }

void Invader::onUpdate() {
	const cv::Size imgSize = invaderImage.size();
	pos.x += mov.x;
	if ((camSize.width <= (pos.x + imgSize.width)) || (pos.x <= 0)) {
		mov.x *= -1;
		pos.x += mov.x;
		pos.y += imgSize.height + std::abs(mov.x);
	};
}

void Invader::onPaint(cv::Mat &image) const {
	invaderImage.paint(image, pos);
}

cv::Rect Invader::getRect() const {
	return cv::Rect(pos, invaderImage.size());
}

Invader::Factory::Factory(const GameImage& invaderImage, const cv::Size& camSize, int speed):
	invaderImage(invaderImage),
	camSize(camSize),
	speed(speed),
	count(0)
{
}

Invader* Invader::Factory::operator()() {
	const unsigned count_ = count++;
	const int xpos = (invaderImage.size().width + speed) * count_;
	return new Invader(invaderImage, camSize, cv::Point(xpos,0), speed);
}
