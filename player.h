#ifndef PLAYER_H_
#define PLAYER_H_

#include<opencv/cxcore.hpp>

#include"graphic.h"

class GameImage;

class Player : public Graphic {
private:
	const GameImage &playerImage;	// player image
	const cv::Size camSize;
	const int yShot;
	cv::Point pos;					// player face position (middle / nose)

	virtual void onUpdate();
	virtual void onPaint(cv::Mat& image) const;
	virtual cv::Rect getRect() const;

public:
	Player(const GameImage &playerImage, const cv::Size &camSize, int yShot);
	virtual ~Player();
	bool isShooting() const;
	cv::Point facePosition() const;
	void setFacePosition(const cv::Point &facePosition);
};

#endif /* PLAYER_H_ */
