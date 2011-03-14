#include "invader.h"

Invader::Invader(const char *filename, const cv::Point &position,const int speed, cv::Mat &img):
	image(img),pos(position){
	this->inv = cv::imread(filename, 1);
	this->inv_mask = cv::imread(filename, 0);
	this->mov.x = speed;
	this->mov.y = inv.size().height + 5;
}

 Invader::~Invader(void){
	inv.release();
	inv_mask.release();
}

void Invader::update(void){
	if (image.size().width  <= (pos.x + inv.size().width + mov.x)) { mov.x *= -1; pos.y += mov.y; };
	if ((pos.x + mov.x) <= 0) { mov.x *= -1; pos.y += mov.y; };
	pos.x += mov.x;
}

void Invader::draw(void){
	cv::Rect roi(pos.x, pos.y, inv.size().width , inv.size().height);
	image_roi = image(roi);
	image_roi.setTo(0,inv_mask);
	image_roi += inv;
	image_roi.release();
}

bool Invader::colision(const cv::Rect &rect){
	cv::Point opt[4];
	opt[0] = cv::Point(rect.x,rect.y);
	opt[1] = cv::Point(rect.x + rect.width, rect.y);
	opt[2] = cv::Point(rect.x,rect.y  + rect.height);
	opt[3] = cv::Point(rect.x + rect.width, rect.y + rect.height);

	cv::Rect myob(pos, inv.size());

	for(unsigned int i = 0; i < 4; i++){
		if (myob.contains(opt[i])) return true;
	}

	return false;
}

