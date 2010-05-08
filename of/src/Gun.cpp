/*
 *  Gun.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 4/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Gun.h"

Gun::Gun(int _x, int _y, Obstacles *_obs){
	bullets.clear();
	pos.set(_x,_y);
	obs = _obs;
	s = 40;
	speed = 2.0;
	timeSinceLastShot = 0;
	direction = 1;
}

void Gun::aim(Player *_p){
	//angle = atan2(_p->pos.x - pos.x, _p->pos.y - pos.y);
	angle += (move * direction);
	//printf("Gun angle = %f\n",angle);
	if((angle < aMin) || (angle > aMax)){
		switchDirections();
	}
}

void Gun::switchDirections(){
	printf("Switching directions\n");
	if(direction == 1){
		direction = -1;
	} else {
		direction = 1;
	}
}

void Gun::update(){
	if(ofGetElapsedTimeMillis() - timeSinceLastShot > SHOT_DELAY){
		ofxVec2f shooting(pos.x + cos(angle) * (s*2),pos.y + sin(angle)*(s*2));
		Bullet bul(angle,speed,shooting);
		//bullets.push_back(bul);
		timeSinceLastShot = ofGetElapsedTimeMillis();
	}
}

void Gun::draw(){
	ofFill();
	ofSetColor((int)c.x, (int)c.y,(int)c.z);
	ofCircle(pos.x, pos.y, s);
	ofSetLineWidth(12);
	ofSetColor(0);
	ofLine(pos.x,pos.y,pos.x + cos(angle) * (s*2),pos.y + sin(angle)*(s*2));
	ofSetLineWidth(1);
	vector<Bullet>::iterator bi;
	for(bi = bullets.begin(); bi != bullets.end(); ++bi){
		bi->update();
		if(obs->isColliding(bi->pos, 5
		   )){
			//bullets.erase(bi);
			continue;
		}
		if(bi->isAlive){
			bi->draw((int)c.x,(int)c.y,(int)c.z);
		} else {
			bullets.erase(bi);
		}
	}
}

Bullet::Bullet(float _angle, float _speed, ofxVec2f _start){
	angle = _angle;
	speed = _speed;
	pos = _start;
	isAlive = 1;
}

void Bullet::update(){
	pos.set(pos.x + cos(angle) * speed, pos.y + sin(angle) * speed);
	if(pos.x < 0 || pos.x > ofGetWidth()){
		isAlive = 0;
	}
	if(pos.y < 0 || pos.y > ofGetHeight()){
		isAlive = 0;
	}
}

void Bullet::draw(int _r,int _g, int _b){
	ofSetColor(_r,_g,_b);
	ofEllipse(pos.x, pos.y, 10, 10);
}

