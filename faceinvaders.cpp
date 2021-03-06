#include <opencv/cv.hpp>
#include <opencv/highgui.h>

#include <deque>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <ctime>

#include "playerposition.h"
#include "graphic.h"
#include "gameimage.h"
#include "player.h"
#include "invader.h"
#include "shot.h"

static const int DEFAULT_DEVICE = 0;
static const int FLIPPING_AROUND_Y_AXIS = 1; ///< flipping around y-axis
static const int SHOT_LINE_PIX = 300;
static const unsigned MAX_INVADERS = 8;
static const int WAIT_DELAY_MS = 10;
static const int SHOT_SPEED = 6;

static const char PLAYER_PNG[] = "./data/player.png";
static const char INVADER_PNG [] = "./data/invader.png";
static const char HAARCASCADE_XML[] = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";

class GraphicPaint {
	cv::Mat &i;
public:
	inline GraphicPaint(cv::Mat &i):i(i){}
	inline void operator()(Graphic &g) { g.paint(i); }
};

static bool checkColision(const cv::Rect &a, const cv::Rect &b){
	if (b.contains(a.tl())) return true;
	if (b.contains(a.br())) return true;
	if (b.contains(cv::Point(a.x+a.width,a.y))) return true;
	if (b.contains(cv::Point(a.x,a.y+a.height))) return true;
	return false;
}

class GraphicColision {
	cv::Rect a;
public:
	inline GraphicColision(const cv::Rect &a):a(a){}
	inline bool operator()(const Graphic &g) const { return checkColision(a,g.rect()); }
};

inline static void GraphicUpdate(Graphic &g) { g.update(); }
inline static bool isInvalidShot(const Shot &s) { return !s.isValid(); }

int main() {
	typedef std::deque<Invader> InvaderList;
	typedef std::deque<Shot> ShotList;

	cv::VideoCapture cap( DEFAULT_DEVICE );
	if(!cap.isOpened()) {
		std::cerr << "Error opening VideoCapture!" << std::endl;
		return -1;
	}
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
	
	cv::namedWindow("SpaceInvaders", CV_WINDOW_AUTOSIZE);
	
	cv::Mat cameraImage;
	cap >> cameraImage;
	
	const cv::Size CameraImageSize( cameraImage.size() );
	
	for (int key = -1; ' ' != key; key = cv::waitKey(WAIT_DELAY_MS)) {
		cap >> cameraImage;
		cv::flip(cameraImage, cameraImage, FLIPPING_AROUND_Y_AXIS);
		cv::putText(cameraImage, "Press space to play!", cv::Point(30,80), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar::all(255), 2, 8);
		cv::resize(cameraImage, cameraImage, CameraImageSize * 2);
		cv::imshow("SpaceInvaders", cameraImage);
	}
	
	const GameImage playerImage( PLAYER_PNG );
	const GameImage invaderImage( INVADER_PNG );
	
	const int YPlayerShot = CameraImageSize.height - playerImage.size().height;
	const int YGameOver = YPlayerShot - invaderImage.size().height;
	
	Player player(playerImage, CameraImageSize, SHOT_LINE_PIX);
	PlayerPosition playerPosition(player, HAARCASCADE_XML, CameraImageSize);
	Shot playerShot;
	bool gameOver = false;
	
	InvaderList invaders;
	std::generate_n(std::back_inserter(invaders), MAX_INVADERS, Invader::Factory(invaderImage, CameraImageSize, 5));
	
	ShotList shots;
	
	cv::RNG rng(uint64(std::time(0)));
	
	for (int key = -1; 'q' != key; key = cv::waitKey(WAIT_DELAY_MS)) {
		cap >> cameraImage;
		cv::flip(cameraImage, cameraImage, FLIPPING_AROUND_Y_AXIS);

		if (!gameOver) {
		
		playerPosition.update(cameraImage);
		playerShot.update();
		std::for_each(invaders.begin(), invaders.end(), GraphicUpdate);
		std::for_each(shots.begin(), shots.end(), GraphicUpdate);
		
		if (playerShot.isValid()) {
			const InvaderList::iterator iInvaderEnd = std::remove_if(invaders.begin(),invaders.end(), GraphicColision(playerShot.rect()));
			if (iInvaderEnd != invaders.end()) {
				invaders.erase(iInvaderEnd, invaders.end());
				playerShot = Shot();
			}
		}
		
		if (!shots.empty()) {
			const ShotList::iterator iShotsEnd = std::remove_if(shots.begin(), shots.end(), isInvalidShot);
			if (iShotsEnd != shots.end()) {
				shots.erase(iShotsEnd, shots.end());
			}
		}
		
		for (InvaderList::const_iterator iInvader = invaders.begin(); iInvader != invaders.end() && !gameOver; ++iInvader) {
			const cv::Rect irect( iInvader->rect() );
			if ((rng.uniform(0.0,1.0) < 0.05) && (shots.size() < MAX_INVADERS)) {
				cv::Point shotPos(irect.x + (irect.width / 2), irect.y + irect.height);
				shots.push_back( Shot(shotPos, SHOT_SPEED, cv::Scalar(100,50,100), CameraImageSize) );
			}
			if (irect.y >= YGameOver) {
				gameOver = true;
			}
		}
		
		if (!playerShot.isValid() && player.isShooting()) {
			cv::Point shotPoint( player.facePosition().x, YPlayerShot );
			playerShot = Shot(shotPoint, -SHOT_SPEED, cv::Scalar(100,170,10), CameraImageSize);
		}
		
		for (ShotList::iterator iShot(shots.begin()); iShot != shots.end() && !gameOver; ++iShot) {
			if (iShot->isValid() && checkColision(iShot->rect(),player.rect())) {
				gameOver = true;
			}
		}
		
		}
		
		std::for_each(invaders.begin(), invaders.end(), GraphicPaint(cameraImage));
		std::for_each(shots.begin(), shots.end(), GraphicPaint(cameraImage));
		player.paint(cameraImage);
		playerShot.paint(cameraImage);
		
		if (invaders.empty()) {
			cv::putText(cameraImage, "Winner!", cv::Point(30,80), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar::all(255), 2, 8);
		} else if (gameOver) {
			cv::putText(cameraImage, "Game Over!", cv::Point(30,80), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar::all(255), 2, 8);
		}
		
		cv::resize(cameraImage, cameraImage, CameraImageSize * 2);
		cv::imshow("SpaceInvaders", cameraImage);
	}
	
	return 0;
}