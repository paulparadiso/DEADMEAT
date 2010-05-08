#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Obstacles.h"
#include "Player.h"
#include "lcd.h"
#include "Eye.h"
#include "Gun.h"
#include <map>
#include <utility>

#define SERIAL_PORT_A "/dev/tty.FireFly-1655-SPP-1"
#define SERIAL_PORT_B "/dev/tty.FireFly-D396-SPP-1" //red
#define FAKING 1
#define HIT_LOSS 5
#define HEADING_DIFF 15.0

class Player;
class Gun;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();	
		void reset();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		Obstacles *obstacles;
		Player *player;
		//Gun *guns[2];
		map<string,char> commandMap;
		ofSerial serial;
		int drawEye;
		Eye *eye;
		ofTrueTypeFont  franklinBook;
		int isRunning;
		string output;
		
		ofVideoGrabber grabber;
		ofxCvColorImage convert;
		ofxCvGrayscaleImage gray;
	
		int width, height, bpp;
		size_t        numPixels;
		size_t        numBytesRead;
		unsigned char *cameraBuffer;
	
		bool bDraw;
		bool useBCH;
		ofTrueTypeFont idNumb;
};

#endif
