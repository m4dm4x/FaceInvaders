/*
 * invader.h
 *
 *  Created on: Apr 11, 2010
 *      Author: filip
 */

#ifndef INVADER_H_
#define INVADER_H_

#include<opencv/cv.h>
#include<opencv/highgui.h>

#include "graphic.h"

class Invader : public Graphic {
private:
	cv::Mat &image;    // Background Image
	cv::Point pos;     // Invader Position
	cv::Point mov;     // Motion Vector
	cv::Mat inv;       // Invader Picture
	cv::Mat inv_mask;  // Invader Mask
	cv::Mat image_roi; // Ivaders Region of Inerest

public:

	Invader(const char *filename, const cv::Point &position, const int speed, cv::Mat &img);
	virtual ~Invader(void);

	void update(void);
	void draw(void);

	cv::Point getPosition(void) {return cv::Point(pos.x + (inv.size().height>>1),pos.y);};
	bool colision(const cv::Rect &sh);

	//TODO: isWinner method
	//bool isWinner(void);
};

#endif /* INVADER_H_ */
