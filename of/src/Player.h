/*
 *  Player.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 1/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ofxVectorMath.h"
#include "ofMain.h"
#include "Obstacles.h"
#include "Mind.h"

class Mind;

class Player{

public:
	Player(Obstacles *_o, ofSerial *_port);
	void draw();
	void update();
	void setPos(float _x, float _y);
	void setPosFromCamera(ofxVec2f *_c_pos, int _cW, int _cH);
	ofxVec2f getPos();
	void setColor(float _r, float _g, float _b);
	void setSize(int _s);
	void setHeading(float _heading);
	void updateHeading();
	int getHeading();
	void setShape();
	void setGoal(float _x, float _y);
	int getWidth();
	int getLength();
	void reset();
	void setOffset();
	void walk();
	int size;
	int ranAstar;
	ofxVec3f color;
	ofxVec2f pos;
	ofxVec2f head;
	ofxVec2f rightHand;
	ofxVec2f leftHand;
	ofxVec2f goal;
	ofSerial *port;
	int isDead;
	int strength;
	int heading;
	float offset;
	int width;
	int length;
	Mind *mind;
	Obstacles *obs;
	
private:

};

#endif
