#include "player.h"

Player::Player(const char *filename, const cv::Point &position,const int shot_y, cv::Mat &img):image(img){
	this->inv = cv::imread(filename, 1);
	this->inv_mask = cv::imread(filename, 0);
	this->shot_y = shot_y;
	this->x_dif = inv.size().width / 2;
	this->pos.y = img.size().height - inv.size().height - 1;
	this->pos.x = 0;
	this->is_shoting = false;
	update(position);
}

Player::~Player(void){
	inv.release();
	inv_mask.release();
}

void Player::update(const cv::Point &newpos){
	if ((newpos.x - x_dif) <= 0 ) {
		pos.x = 0;
	} else if ((newpos.x + x_dif) >= image.size().width) {
		pos.x = image.size().width - x_dif - 1;
	} else {
		pos.x = newpos.x - x_dif;
	}

	this->is_shoting = newpos.y < shot_y ?  true : false;
}

void Player::draw(void){
	cv::Rect roi(pos.x, pos.y, inv.size().width , inv.size().height);
	image_roi = image(roi);
	image_roi.setTo(0,inv_mask);
	image_roi += inv;
	image_roi.release();
}

bool Player::isShoting(void){
	return this->is_shoting;
}

cv::Point Player::getShotPoint(void){
	if (!is_shoting) return cv::Point(-10, -10);
	return cv::Point(pos.x + x_dif, pos.y);
}

bool Player::colision(const cv::Rect &rect){
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
