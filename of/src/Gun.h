/*
 *  Gun.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 4/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _GUN_H_
#define _GUN_H_
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "Obstacles.h"
#include "Player.h"
#include <vector>


#define SHOT_DELAY 2000

class Player;


class Bullet{
public:
	Bullet(float _angle, float _speed, ofxVec2f _start);
	void update();
	void draw(int _r, int _g, int _b);
	ofxVec2f pos;
	int isAlive;
	float angle;
	float speed;
};

class Gun{
public:
	Gun(int _x, int _y, Obstacles *_obs);
	void aim(Player *_p);
	void update();
	void draw();
	void updateBullets();
	void shoot();
	Obstacles *obs;
	ofxVec2f pos;
	void setPos(float _x, float _y){pos.set(_x,_y);}
	float angle;
	vector<Bullet>bullets;
	ofxVec3f c;
	void setColor(float _r, float _g, float _b){c.set(_r,_g,_b);}
	float move;
	void setMove(float _move){move = _move;}
	int direction;
	void switchDirections();
	int s;
	int timeSinceLastShot;
	void setSize(int _s){s=_s;}
	float aMin;
	float aMax;
	void setMinMax(float _min, float _max){aMin = _min; aMax = _max; angle = aMin;}
	float speed;
};
#endif

