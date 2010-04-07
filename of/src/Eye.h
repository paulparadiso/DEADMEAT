/*
 *  Eye.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 4/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _EYE_H_
#define _EYE_H_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"


#define NUM_COLORS 2
#define CAM_ID 3

class color
{
public:
	float hue, sat;
	ofxVec2f pos;
};


class Eye{
public:
	
	int camWidth;
	int camHeight;
	
	Eye(int _cW, int _cH);
	void trackColors();
	void update();
	ofxVec2f * getColorPos(int _i);
	void setColor(int _x, int _y, int _button);
	void draw();
	
	ofVideoGrabber grabber;
	ofxCvColorImage cImg;
	ofxCvColorImage camImg;
	ofxCvColorImage cImgHSV;
	
	ofxCvGrayscaleImage	hueImg;     
	ofxCvGrayscaleImage	satImg;  
	ofxCvGrayscaleImage briImg;
	
	ofxCvGrayscaleImage colors[NUM_COLORS];
	
	color colorsToTrack[NUM_COLORS];
	int colorIndex;
	int timeSinceNetCheck;
	
	unsigned char * colorTrackedPixels[NUM_COLORS];
	ofTexture trackedTexture[NUM_COLORS];
	
	ofxCvContourFinder finder[NUM_COLORS];
};

#endif

