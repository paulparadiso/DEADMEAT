/*
 *  Player.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 1/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Player.h"

Player::Player(Obstacles *_o, ofSerial *_port){
	//setPos(50,400);
	//setGoal(ofGetWidth()-40,ofGetHeight()/2 + 10);
	//setGoal(10,40);
	//setColor(255,0,0);
	setSize(20);
	port = _port;
	//setHeading(0);
	heading = 0;
	width = 40;
	length = 40;
	setShape();
	obs = _o;
	printf("Player position = %d,%d\n",(int)pos.x,pos.y);
	mind = new Mind(this,obs);
	ranAstar = 0;
	isDead = 1;
	//mind->update();
}

void Player::update(){
	mind->update();	
}

void Player::draw(){
	ofSetColor(color.x, color.y, color.z);
	ofFill();
	ofCircle(pos.x, pos.y, 10);
	setShape();
	//ofLine(pos.x,pos.y,pos.x + (cos(ofDegToRad(heading)) * length), pos.y + (sin(ofDegToRad(heading)) * length));
	//ofLine(leftHand.x,leftHand.y,rightHand.x,rightHand.y);
	ofNoFill();
	ofSetLineWidth(2);
	ofTriangle(head.x,head.y, rightHand.x,rightHand.y,leftHand.x,leftHand.y);
	ofSetLineWidth(1);
	mind->draw();
}

void Player::reset(){
	strength = 90;
	isDead = 0;
}

void Player::setShape(){
	head.set(pos.x + (cos(ofDegToRad(heading)) * length), pos.y + (sin(ofDegToRad(heading)) * length));
	rightHand.set(pos.x + (cos(ofDegToRad(heading + 90)) * width/2), pos.y + (sin(ofDegToRad(heading +90)) * width/2));
	leftHand.set(pos.x + (cos(ofDegToRad(heading + 270)) * width/2), pos.y + (sin(ofDegToRad(heading +270)) * width/2));
}

void Player::setPos(float _x, float _y){
	pos.set(_x,_y);
}

void Player::setPosFromCamera(ofxVec2f *_c_pos, int _cW, int _cH){
	float tmpX = _c_pos->x / (float)_cW;
	float tmpY = _c_pos->y / (float)_cH;
	setPos((float)ofGetWidth() * tmpX, (float)ofGetHeight() * tmpY);
	
}

void Player::setColor(float _r, float _g, float _b){
	color.set(_r,_g,_g);
}

void Player::setSize(int _s){
	size = _s;
}

ofxVec2f Player::getPos(){
	return pos;
}

void Player::setGoal(float _x, float _y){
	goal.set(_x,_y);
}

void Player::updateHeading(){
	/*
	 Request current heading of human player.
	 */
	if(!FAKING){			
		string newAngle;
		int newHeading;
		unsigned char newByte = 0;
		port->writeByte('e');
		ofSleepMillis(100);
		while(newByte != '\n'){
			newByte = port->readByte();
			if(newByte != 0xff){
				newAngle.append((char *)&newByte,1);
			}
		}
		istringstream stream(newAngle);
		stream >> newHeading;
		if(newHeading != 0)
			heading = newHeading;
		newAngle.clear();
	}
}

int Player::getHeading(){
	return heading;
}

void Player::setOffset(){
	offset = heading;
}

void Player::setHeading(float _heading){
	/*
	 Tell player which direction to turn.
	 */
	if(!FAKING){
		if(_heading < heading){
			port->writeByte('f');
		} else {
			port->writeByte('g');
		}
	} else {
		heading = _heading;
	}
}

void Player::walk(){
	port->writeByte('h');
}

int Player::getWidth(){
	return width;
}

int Player::getLength(){
	return length;
}
