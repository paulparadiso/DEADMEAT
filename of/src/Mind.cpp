/*
 *  Mind.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Mind.h"

Mind::Mind(Player *_p, Obstacles *_o){
	body = _p;
	obs = _o;
	astar = new Astar2(this,ofGetWidth(),ofGetHeight());
	lastPos.set(0.0,0.0);
	isSearching = 1;
	timeSinceLastCommand = 0;
}

void Mind::update(ofxVec2f _goal){
	//ofxVec2f futurePos;
	//Code goes here.
	//body->pos = futurePos;
	if(isSearching){
		//printf("Starting search\n");
		if(astar->run(_goal)){
			isSearching = 0;
			//printf("ASTAR SUCCESS\n");
			lastPos = body->pos;
		} 	
	}
	if(abs(body->pos.x - lastPos.x) > STEP_SIZE || abs(body->pos.y - lastPos.y) > STEP_SIZE){
		if(!isSearching){
			isSearching = 1;
			//printf("Turning on a*\n");
		//printf("RAN ASTAR\n");
		//draw();
		}
	}
	if(astar->pathGenerated){
		if(ofGetElapsedTimeMillis() - timeSinceLastCommand > MOVE_DELAY){
			ofxVec2f nearest = getNearestPoint();
			guidePlayer(&nearest);
			timeSinceLastCommand = ofGetElapsedTimeMillis();
		}
	}
	if(obs->isColliding(body->pos, 5)){
		body->strength -= HIT_LOSS;
	}
	if(body->strength <= 0)
		body->isDead = 1;
}

ofxVec2f Mind::getNearestPoint(){
	vector<ofxVec2f>::iterator pi;
	ofxVec2f target = ofxVec2f(0.0,0.0);
	int dst = TOLERANCE;
	for(pi = (*astar).path.end(); pi != (*astar).path.begin(); --pi){
		float tmpDst = body->pos.distance((*pi));
		if(tmpDst < dst){
			target = (*pi);
			dst = tmpDst;
		}
	}
	return target;
}

void Mind::guidePlayer(ofxVec2f *_target){
	float angle = atan2(body->pos.x - _target->x, body->pos.y - _target->y);
	//printf("Setting heading to %f\n",angle);
	if(!FAKING){
		if(abs(angle - (DEG_TO_RAD * (body->heading - body->offset)) < HEADING_DIFF)){
			body->walk();
		} else {
			body->setHeading(angle * RAD_TO_DEG);
		}
	}
	body->setHeading(angle * RAD_TO_DEG);
}

void Mind::draw(){
	//printf("Mind drawing.\n");
	ofSetColor(body->color.x, body->color.y, body->color.z);
	vector<ofxVec2f>::iterator iter;
	for(iter = astar->path.begin();iter != astar->path.end()-1;iter++){
		ofLine(iter->x, iter->y, (iter+1)->x, (iter+1)->y);
	}
}