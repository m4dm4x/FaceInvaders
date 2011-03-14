//============================================================================
// Name        : fpaceinvaders.cpp
// Version     : 0.0.5 PreAlpha
// Copyright   : Public Domain
//============================================================================

//#define SAVE_VIDEO
//#define NOINVADERS
#define HAARCASCADE_PATH "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include <iostream>
#include <list>

#include "graphic.h"
#include "invader.h"
#include "player.h"
#include "shot.h"

using namespace std;
using namespace cv;

void inline drawCross(Mat &img, const Point &pos);
bool inline shotIsOut(Shot *sht);

int main() {
#ifdef SAVE_VIDEO
	VideoWriter writer("face_invaders.avi",CV_FOURCC('P','I','M','1'), 25, Size(300,200), 1);
#endif

	typedef list<Invader*>::iterator InvIT;
	typedef list<Shot*>::iterator ShotIT;

	cv::RNG rng(0xfffffff);

	const int MAX_INVADERS = 8;
	const int shotline = 80;

	VideoCapture cap( 0 ); // open the default camera
	if(!cap.isOpened()) {
		cerr << "Error opening capture device!!";
		return -1;
	}

	CascadeClassifier cascade;
	if ( !cascade.load( HAARCASCADE_PATH ) ) {
		cerr << "Error loading haarcascades!";
		return -1;
	}

	namedWindow("SpaceInvaders",CV_WINDOW_AUTOSIZE);
	Mat bg;
	cap >> bg;
	Mat cam(Size(bg.size().width>>1, bg.size().height>>1), CV_8UC1);
	Mat cgray(cam.size(),CV_8UC1);

#ifndef NOINVADERS

	list<Invader*> invaders;
	for (int i = 0; i < MAX_INVADERS; i++) {
		Invader *invader = new Invader("./data/invader.png", Point(0 + 65*i, 80), 5, bg);
		invaders.push_back(invader);
	}

	list<Shot*> ishots;

	Player player("./data/player.png", Point(200,80), shotline, bg);
	Shot player_shot(Point(-10,-10), -10, Scalar(200,200,200), bg); player_shot.update();
#endif

	vector<Rect> faces;

	int key = 0;
	while ((char)key != 'q'){

		cap >> bg;
		flip(bg, bg, 1);
		resize(bg,cam,cam.size());
		cvtColor( cam, cgray, CV_BGR2GRAY );
		equalizeHist(cgray, cgray);

		faces.clear();
		cascade.detectMultiScale( cgray, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		if (faces.size() > 0) {
			Point facepoint((faces[0].x + (faces[0].width>>1))<<1, (faces[0].y + (faces[0].height>>1))<<1);
			drawCross(bg, facepoint);

#ifndef NOINVADERS
			player.update(facepoint);
			if (player.isShoting()) {
				if (player_shot.isOut()) player_shot.update(player.getShotPoint());
			}
		}

		//Object Updates
		player_shot.update();
		for (ShotIT it = ishots.begin(); it != ishots.end(); it++) {
			(*it)->update();
		}
		ishots.remove_if(shotIsOut);


		for (InvIT it = invaders.begin(); it != invaders.end(); it++) {
			(*it)->update();
			if (ishots.size() < invaders.size()) {
				if (rng.uniform(0.0, 1.999999) < 0.1) {
					Shot *is = new Shot((*it)->getPosition(), 8, Scalar(000,000,250), bg);
					ishots.push_back(is);
				}
			}
		}


		//Colision Detection
		Rect shot_rect = player_shot.getRectangle();
		for (InvIT it = invaders.begin(); it != invaders.end(); it++) {
			if ((*it)->colision(shot_rect)) {
				player_shot.setOut();
				delete (*it);
				invaders.remove((*it));
				break;
			}
		}
		for (ShotIT it = ishots.begin(); it != ishots.end(); it++) {
			if (player.colision((*it)->getRectangle())) {
				invaders.clear();
				cout << "GAME OVER\n";
				break;
			}
		}

		// Graphic Output :o)
		line(bg, Point(0, shotline), Point(bg.size().width, shotline),Scalar(50,55,55),2,1);
		player_shot.draw();
		for (ShotIT it = ishots.begin(); it != ishots.end(); it++) {
			(*it)->draw();
		}
		player.draw();
		for (InvIT it = invaders.begin(); it != invaders.end(); it++) {
			(*it)->draw();
		}

		if (invaders.empty()) break;

#endif
#ifdef NOINVADERS
		}
#endif
		imshow("SpaceInvaders", bg);
#ifdef SAVE_VIDEO
		writer << bg;
#endif
		key = waitKey(10);

	}

#ifndef NOINVADERS
	for (InvIT it = invaders.begin(); it != invaders.end(); it++) {
				delete (*it);
	}
#endif
	return 0;
}

void inline drawCross(Mat &img, const Point &pos){
	line(img, Point(pos.x - 10, pos.y), Point(pos.x + 10, pos.y),Scalar(255,255,255),2,1);
	line(img, Point(pos.x, pos.y - 10), Point(pos.x, pos.y + 10),Scalar(255,255,255),2,1);
}

bool inline shotIsOut(Shot *sht){
	bool out = sht->isOut();
	if (out) delete sht;
	return out;
}

