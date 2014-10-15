#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <opencv/cv.hpp>

class Graphic {
public:
  virtual ~Graphic() { }
  void paint(cv::Mat &image) const { onPaint(image); }
  void update() { onUpdate(); }
  cv::Rect rect() const { return getRect(); }
private:
  virtual void onPaint(cv::Mat &image) const = 0;
  virtual void onUpdate() = 0;
	virtual cv::Rect getRect() const = 0;
};

#endif /* GRAPHIC_H_ */
