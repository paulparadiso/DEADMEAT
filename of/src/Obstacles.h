/*
 *  Obstacle.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 1/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OBSTACLES_H_
#define _OBSTACLES_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ofxVectorMath.h"
#include "ofMain.h"

#define EDGE_SIZE 20

class Obstacles{

public:
	Obstacles();
	void makeObstacles(string _inFile);
	void draw();
	void setColor(float _r, float _g, float _b);
	vector <ofxVec2f> allPoints;
	int isWalkable(int _x, int _y);
	int edge;

private:
	void addObstacle(ofxVec2f _a, ofxVec2f _b, ofxVec2f _c);
	void makePoints();
	int getCommon(ofxVec2f _a, ofxVec2f _b);	
	typedef vector <ofxVec2f> obPoints;
	vector <obPoints> allObstacles;
	
	ofxVec3f color;
};

#endif

