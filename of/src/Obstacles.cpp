/*
 *  Obstacle.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 1/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "Obstacles.h"

Obstacles::Obstacles(){
	edge = EDGE_SIZE;
	setColor(0,255,0);
}

void Obstacles::setColor(float _r, float _g, float _b){
	color.set(_r,_g,_b);
}

void Obstacles::makeObstacles(string _inFile){
	ifstream theFile(_inFile.c_str());
	vector <string> splitReturn;
	obPoints newPoints;
	string line;
	if(theFile.is_open()){
		while(!theFile.eof()){
			getline(theFile,line);
			//cout<<line<<endl;
			splitReturn = ofSplitString(line, ",");
			vector<string>::iterator iter;
			newPoints.clear();
			for(iter = splitReturn.begin();iter != splitReturn.end();iter++){
				//cout<<*iter<<endl;
				int a = ofToInt(*(iter++));
				int b = ofToInt(*(iter));
				newPoints.push_back(ofxVec2f(a,b));
			}
			allObstacles.push_back(newPoints);
		}
		cout<<"All obstacles had "<<allObstacles.size()<<endl;
		makePoints();
		draw();
	} else {
		cout<<"NO FILE!"<<endl;
	}
}

void Obstacles::draw(){
	ofSetColor(color.x,color.y,color.z);
	vector<ofxVec2f>::iterator iter;
	for(iter = allPoints.begin(); iter != allPoints.end(); iter++){
		//cout << (*iter).x << "  " << (*iter).y << endl;
		ofFill();
		ofRect((*iter).x,(*iter).y,edge,edge);
	}
}

int Obstacles::isWalkable(int _xA, int _yA, int _xB, int _yB, int _sz){
	if(_xA <= 0 || _xA >= ofGetWidth())
		return 0;
	if(_yA <= 0 || _yA >= ofGetHeight())
		return 0;
	int inX = 0;
	int inY = 0;
	ofxVec2f *tst = new ofxVec2f(_xA,_yA);
	vector<ofxVec2f>::iterator vecIter;
	for(vecIter = allPoints.begin();vecIter != allPoints.end(); vecIter++){
		ofxVec2f tt = *(vecIter);
		if(_xA < _xB){
			if(tt.x > _xA && tt.x < _xB)
				inX = 1;
		} else {
			if(tt.x > _xB && tt.x < _xA)
				inX = 1;
		}
		if(_yA < _yB){
			if(tt.x > _yA && tt.x < _yB)
				inY = 1;
		} else {
			if(tt.x > _yB && tt.x < _yA)
				inY = 1;
		}
		if(inX && inY)
			return 0;
		if(tst->distance(tt) < edge)
			return 0;
	}
	return 1;
}

int Obstacles::isColliding(ofxVec2f _tst, int _e){
	vector<ofxVec2f>::iterator vecIter;
	for(vecIter = allPoints.begin();vecIter != allPoints.end(); vecIter++){
		ofxVec2f tt = *(vecIter);
		if(_tst.distance(tt) < _e)
			return 1;
	}
	return 0;
}

int Obstacles::getCommon(ofxVec2f _a, ofxVec2f _b){
	if((_a.y == _b.y) && (_a.x != _b.x)){
		//cout << "COMMON" << endl;
		return 1;
	} else {
		return 0;
	}
}

void Obstacles::makePoints(){
	vector<obPoints>::iterator iter;
	vector<ofxVec2f>::iterator vecIter;
	int i;
	for(iter = allObstacles.begin(); iter != allObstacles.end(); iter++){
		//cout << "starting iter..." << endl;
		obPoints obs = *iter;
		for(vecIter = obs.begin(); vecIter != obs.end() - 1; vecIter++){
			ofxVec2f a = *(vecIter);
			ofxVec2f b = *(vecIter + 1);
			//cout << "A = " << a.x << "," << a.y << endl;
			//cout << "B = " << b.x << "," << b.y << endl;
			int c = getCommon(a,b);
			if(c == 0){
				if(a.y < b.y){
					for(i = a.y; i < b.y; i += edge){
						//cout << "Adding point " << a.x << "," << i << endl;
						allPoints.push_back(ofxVec2f(a.x,i));
					}
				} else {
					for(i = a.y; i > b.y; i -= edge){
						//cout << "Adding point " << a.x << "," << i << endl;
						allPoints.push_back(ofxVec2f(a.x,i));
					}
				}
			} else {
				if(a.x < b.x){
					for(i = a.x; i < b.x; i += edge){
						//cout << "Adding point " << i << "," << a.y << endl;
						allPoints.push_back(ofxVec2f(i,a.y));
					}
				} else {
					for(i = a.x; i > b.x; i -= edge){
						//cout << "Adding point " << i << "," << a.y << endl;
						allPoints.push_back(ofxVec2f(i,a.y));
					}
				}
			}
		}
	}
	for(vecIter = allPoints.begin();vecIter != allPoints.end(); vecIter++){
		ofxVec2f tt = *(vecIter);
		//cout << tt.x << "," << tt.y << endl;
	}
}
		