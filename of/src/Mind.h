/*
 *  Mind.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _MIND_H_
#define _MIND_H_

#include "Player.h"
#include "Search.h"
#include "Obstacles.h"
#include "ofxVectorMath.h"
#include "ofMain.h"
#include "testApp.h"
#include <vector>

#define STEP_SIZE 20
#define MAX_SEARCH 300
//distance player can wander from path before new path is generated.
#define TOLERANCE 20
#define MOVE_DELAY 2000

class Player;
class Astar;
class Astar2;

class Mind{
public:
	Mind(Player *_p, Obstacles *_o);
	void update();
	void draw();
	int getHeading();
	int checkHeading();
	int isSearching;
	int timeSinceLastCommand;
	ofxVec2f getNearestPoint();
	void guidePlayer(ofxVec2f *_target);
	ofxVec2f lastPos;
	Obstacles *obs;
	Player *body;
	Astar2 *astar;
};

#endif
