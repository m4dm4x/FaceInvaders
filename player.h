#ifndef PLAYER_H_
#define PLAYER_H_

#include<opencv/highgui.h>
#include<opencv/cv.h>

#include"graphic.h"

class Player : public Graphic{
private:
	cv::Mat &image;      //Background Image

	cv::Point pos;       //Player position
	int x_dif;       //Player width/2
	int shot_y;      //Player y-Position for Shooting

	cv::Mat inv;	     //Player Image
	cv::Mat inv_mask;    //Player Image-Mask
	cv::Mat image_roi;   //Players Region of Interrest

	bool is_shoting; //Player Shooting

public:
	Player(const char *filename, const cv::Point &position,const int shot_y, cv::Mat &img);
	virtual ~Player(void);

	void update(void){};
	void update(const cv::Point &newpos);
	void draw(void);

	bool isShoting(void);
	cv::Point getShotPoint(void);
	bool colision(const cv::Rect &sh);
};

#endif /* PLAYER_H_ */
