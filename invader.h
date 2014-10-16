#ifndef INVADER_H_
#define INVADER_H_

#include <opencv/cxcore.hpp>
#include "graphic.h"
#include "gameimage.h"

class Invader : public Graphic {
private:
	GameImage invaderImage;	// player image
	cv::Size camSize;
	cv::Point pos;     // Invader Position
	cv::Point mov;     // Motion Vector
  
	virtual void onPaint(cv::Mat &image) const;
	virtual void onUpdate();
	virtual cv::Rect getRect() const;

public:
	Invader(const GameImage &invaderImage, const cv::Size &camSize, const cv::Point &position, int speed);
	virtual ~Invader();

//	cv::Point getPosition() const;
//	bool colision(const cv::Rect &sh);

	class Factory;
};

class Invader::Factory {
private:
	const GameImage &invaderImage;
	const cv::Size &camSize;
	const int speed;
	unsigned count;
public:
	Factory(const GameImage &invaderImage, const cv::Size &camSize, int speed);
	Invader operator()();
};

#endif /* INVADER_H_ */
