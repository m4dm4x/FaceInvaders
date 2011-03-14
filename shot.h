/*
 * shot.h
 *
 *  Created on: Apr 11, 2010
 *      Author: filip
 */

#ifndef SHOT_H_
#define SHOT_H_

#include<opencv/highgui.h>
#include<opencv/cv.h>

#include"graphic.h"

class Shot : public Graphic{
private:
	cv::Point position; //Shot upper position
	int direction;  //Shot Direction (positive = down / negative = up)
	cv::Scalar color;   //Shot color
	cv::Mat &image;     //Background Image
	bool out;       //Shot is out...

	static const int HEIGHT = 20;
public:
	Shot(const cv::Point &position, const int &direction, const cv::Scalar &color, cv::Mat &image);
	virtual ~Shot(void);

	void update(void);
	void update(const cv::Point &newpos);

	void draw(void);

	cv::Rect getRectangle(void);
	void setOut(void);
	bool isOut(void);
};

#endif /* SHOT_H_ */
