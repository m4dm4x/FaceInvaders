#ifndef SHOT_H_
#define SHOT_H_

#include<opencv/cv.hpp>

#include"graphic.h"

class Shot : public Graphic {
private:
	cv::Point position; ///< top position
	int speed; ///< speed direction ( speed > 0  = down / speed < 0 = up)
	cv::Scalar color; ///< shot color
	int height; ///< paint image height (to detect out of image shots)

	virtual void onUpdate();
	virtual void onPaint(cv::Mat& image) const;
	virtual cv::Rect getRect() const;

public:
	Shot();
	Shot(const cv::Point &position, int speed, const cv::Scalar &color, const cv::Size &size);
	virtual ~Shot();
	bool isValid() const;
};

#endif /* SHOT_H_ */
