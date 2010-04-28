/*
 *  Search.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "ofxVectorMath.h"
#include "Obstacles.h"
#include "Mind.h"
#include <vector>

#define MAX_LIST 1000
#define Y_MAX 15
#define X_MAX 15
#define MIN_COST (double)1.0
#define ALPHA (double)0.5
#define GET 0
#define REMOVE 1

class Mind;

struct node_s;

typedef struct node_s{
	struct node_s *parent;
	double g;
	double h;
	double f;
	int x;
	int y;
} node_t;
	
typedef struct {
	int num_elems;
	node_t *elem[MAX_LIST];
} list_t;

const struct {
	int x;
	int y;
} succ[8] = {{0,-1},{0,1},{1,0},{-1,0},{-1,-1},{1,1},{-1,1},{1,-1}};

class Node{
public:
	Node(int _x, int _y);
	
	ofxVec2f pos;
	ofxVec3f vals;
	
	int onOpenedList;
	int onClosedList;
	
	int getX(){return (int)pos.x;}
	int getY(){return (int)pos.y;}
	float getF(){return vals.x;}
	float getG(){return vals.y;}
	float getH(){return vals.z;}
	
	void setX(int _x){pos.x = _x;}
	void setY(int _y){pos.y = _y;}
	void setF(float _f){vals.x = _f;}
	void setG(float _g){vals.y = _g;}	
	void setH(float _h){vals.z = _h;}
	
	int isOnOpenedList(){return onOpenedList;}
	int isOnClosedList(){return onClosedList;}
	void putOnOpenedList(int _b){onOpenedList = _b;}
	void putOnClosedList(int _b){onClosedList = _b;}
	
	Node *parent;
};

typedef vector<Node*> nodeList;

class Astar2{
public:
	Astar2(Mind *_mind, int _w, int _h);
	int run(ofxVec2f _goal);
	double calculateG(Node* _node, int _i);
	double calculateH(Node* _node);
	Node * getSuccessorNode(Node *_curr_node, int _i);
	void buildBestPath(Node *_walker);
	void initSpace();
	Node * spaceGet(int _x, int _y);
	void spaceSet(int _x, int _y, Node _node);
	Node * spaceFindBest();
	Node * listFindBest();
	int isEmpty;
	int atStart;
	nodeList openedList;
	ofxVec2f nearestPoint(ofxVec2f _cPos, int _div);
	void listInsert(nodeList *_list, Node *_node);
	ofxVec2f currPos;
	ofxVec2f lastPos;
	ofxVec2f goal;
	Mind *mind;
	int isGoalNode(Node *_node);
	int numSearched;
	int pathGenerated;
	nodeList closedList;
	vector <ofxVec2f> path;
	vector<Node>space;
	char *grid;
	int w,h;
	int edge;
	double minCost;
	double alpha;
	int spaceInited;
	float cost[8];
};


#endif
