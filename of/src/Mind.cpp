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
	astar = new Astar(this,ofGetWidth(),ofGetHeight());
}

void Mind::update(){
	//ofxVec2f futurePos;
	//Code goes here.
	//body->pos = futurePos;
	if(astar->run(body->goal)){
		printf("RAN ASTAR\n");
		draw();
	}
	else {
		printf("ASTAR FAIL\n");
	}
	
}

int Mind::checkHeading(){
}

void Mind::draw(){
	vector<ofxVec2f>::iterator iter;
	for(iter = astar->path.begin();iter != astar->path.end()-1;iter++){
		ofLine((*iter).x, (*iter).y, (*iter+1).x, (*iter+1).y);
	}
}